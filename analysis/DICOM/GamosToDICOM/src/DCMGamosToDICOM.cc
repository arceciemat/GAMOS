#include "DCMGamosToDICOM.hh"
#include "DICOM/DICOMBase/include/DicomVerbosity.hh"
#include "DICOM/DICOMBase/include/DicomParameterMgr.hh"
#include "DICOM/DICOMBase/include/DicomMgr.hh"
#include "DICOM/DICOMBase/include/DicomVImage.hh"
#include "DICOM/DICOMReaders/include/DicomReaderMgr.hh"
#include "DICOM/DICOMReaders/include/DicomUIDInfo.hh"

#include "GamosCore/GamosReadDICOM/include/GmSqdose.hh"
#include "GamosCore/GamosReadDICOM/include/GmSqdoseHeader.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"

#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmrt/drtplan.h"
#include "dcmtk/dcmrt/drtdose.h"
#include "dcmtk/dcmdata/dcdatset.h"
#include <iostream>
#include <set>
#include <map>
#include "dcmtk/config/osconfig.h"    //make sure OS specific configuration is included first
#include "dcmtk/dcmrt/drttypes.h"
#include "dcmtk/ofstd/ofstd.h"
#include "dcmtk/ofstd/ofstdinc.h"
#include "dcmtk/dcmdata/dcvrui.h"

using namespace std;

//------------------------------------------------------------------------
DCMGamosToDICOM::DCMGamosToDICOM()
{
  theParamMgr = (DicomParameterMgr*)(DicomParameterMgr::GetInstance());
  theReaderMgr = DicomReaderMgr::GetInstance();
  theDicomMgr = DicomMgr::GetInstance();

  theSmallestValue = DBL_MAX;
  theBiggestValue = -DBL_MAX;
}

//------------------------------------------------------------------------
void DCMGamosToDICOM::ProcessArguments( int argc, char** argv ) 
{
  theExeName = "gamosToDICOM";

  //--------- PROCESS ARGUMENTS
  if( argc == 2 ) {
    if( G4String(argv[1]) == "-h" || G4String(argv[1]) == "-help" ) {
       PrintHelp();
       exit(1);
    } else {
      G4Exception(theExeName,
		  "",
		  FatalErrorInArgument,
		  "YOU HAVE TO PROVIDE ARGUMENTS");
      PrintHelp();
    }
  } else if( argc == 1 ) {
    PrintHelp();
    G4Exception(theExeName,
		"",
		FatalErrorInArgument,
		"NO ARGUMENT PROVIDED");
  } else {
    if(argc%2 != 1) { 
      G4Exception(theExeName,
		  "",
		  FatalErrorInArgument,
		  "WRONG NUMBER OF ARGUMENTS: THEY MUST BE -XX1 VAL_XX1 -XX2 VAL_XX2 ... ");
    }

    for( G4int ii = 1; ii < argc; ii++ ){
      G4String argvstr = argv[ii];
      G4String argvstr1 = argv[ii+1];
      G4String argvName = argvstr.substr(1,999);
      if( argvstr == "-patientPosition" ) {
	theParamMgr->AddParam( argvName + " " + argvstr1, PTstring );
	ii++;
      } else if( argvstr == "-fDensityToHU" ) {
	theParamMgr->AddParam( argvName + " " + argvstr1, PTstring );
	ii++;
      } else if( argvstr == "-saveEachZSlice" ) {
	theParamMgr->AddParam( argvName + " " + argvstr1, PTdouble );
	ii++;
      } else if( argvstr == "-suffix" ) {
	theParamMgr->AddParam( argvName + " " + argvstr1, PTstring );
	ii++;
      } else {
	G4int iAddPar = theParamMgr->ReadParameter( argv, ii );
	if( iAddPar == -1 ) {
	  PrintHelp();
	  G4Exception(theExeName.c_str(),
		      "Wrong argument",
		      FatalErrorInArgument,
		      (G4String("ARGUMENT: ") + argv[ii]).c_str());
	}
	ii += iAddPar;
      }
    }
  }
  thePatientPosition = theParamMgr->GetStringValue("patientPosition","HFS");
  theSuffix = theParamMgr->GetStringValue("suffix","dcm");
  if( theSuffix != "" ) theSuffix = "."+theSuffix;

}

//---------------------------------------------------------------------------
void DCMGamosToDICOM::CheckArguments()
{
  //------ At least 1 image file
  size_t nimg = theParamMgr->GetNofImageFiles("fG4dcmCT");
  nimg += theParamMgr->GetNofImageFiles("fG4dcmNM");
  nimg += theParamMgr->GetNofImageFiles("fG4dcmDose");
  nimg += theParamMgr->GetNofImageFiles("fSqdose");
  nimg += theParamMgr->GetNofImageFiles("fInterfile");
  if( nimg != 1 ) {
    G4Exception(theExeName.c_str(),
		"",
		FatalException,
		"THERE MUST BE ONE AND ONLY ONE IMAGE INPUT FILE: G4dcmCT/G4dcmNM/G4dcmDose/Sqdose/Interfile");
  }

  if( theParamMgr->GetNofImageFiles("fCT") > 0
      || theParamMgr->GetNofImageFiles("fNM") > 0
      || theParamMgr->GetNofImageFiles("fRTDose") > 0
      || theParamMgr->GetNofImageFiles("fRTStruct") > 0 ) {
    G4Exception("DCMGamosToDICOM::CheckArguments","",FatalException,"THERE CANNOT BE AS INPUT ANY IMAGE FILE OF TYPE CT/NM/RTDOSE/RTSTRUCT");
  }
 
 theOutputFN = theParamMgr->GetStringValue("fOut","");
  if( theOutputFN == "" ) {
    G4Exception(theExeName.c_str(),
		"",
		FatalException,
		"THERE MUST BE AN OUTPUT FILE, USE -fOut");
  }
  
}

//---------------------------------------------------------------------------
void DCMGamosToDICOM::ConvertImages()
{
  //------- Read images
  theReaderMgr = DicomReaderMgr::GetInstance();
  //  theReaderMgr->ProcessData();
  theReaderMgr->CreateReaders();
  theReaderMgr->SetCTOnlyHU(true);
  theReaderMgr->CreateImages();
  theDicomMgr->OperateAll();

    //-----  Get DicomUIDInfo from one of the DICOM files
  //--- READ DICOM FILES FOR UID
  theDicomUIDInfo = 0;
  std::vector<DicomVReaderImage*> VRTFiles = theReaderMgr->GetImageReaders(DRM_RTDose, false);
  if( VRTFiles.size() != 0 ) {
    theDicomUIDInfo = VRTFiles[0]->GetUIDInfo();
    bUIDfromRT = true;
  }
  //  G4cout << VRTFiles.size() << "RTDose Readers to VRTFiles " << VRTFiles.size() << " theDicomUIDInfo " << theDicomUIDInfo << G4endl; //GDEB
  if( theDicomUIDInfo == 0 ) {
    VRTFiles = theReaderMgr->GetImageReaders(DRM_CT, false);
    if( VRTFiles.size() != 0 ) {
      theDicomUIDInfo =  VRTFiles[0]->GetUIDInfo();
      bUIDfromRT = false;
    }
  }
  //  G4cout << VRTFiles.size() << "CT Readers to VRTFiles " << VRTFiles.size() << " theDicomUIDInfo " << theDicomUIDInfo << G4endl; //GDEB
  if( theDicomUIDInfo == 0 ) {
    VRTFiles = theReaderMgr->GetImageReaders(DRM_NM, false);
    if( VRTFiles.size() != 0 ) {
      theDicomUIDInfo =  VRTFiles[0]->GetUIDInfo();
      bUIDfromRT = false;
    }
  }

  DcmDataset* dataset = new DcmDataset();
  DIModality modality;
  
  std::vector<DicomVImage*> imgs = theDicomMgr->GetImages(DIM_G4dcmCT_MateDens,false);
  if( imgs.size() == 1 ) {
    DicomVImage* g4dcmCTImage = imgs[0];
    // Convert density to HU
    G4String densityToHUFN = theParamMgr->GetStringValue("fDensityToHU","");
    if( densityToHUFN == "" ) {
      G4Exception(theExeName.c_str(),
		  "",
		  FatalException,
		"THERE MUST BE A FILE DEFINING densityToHU CONVERSION, USE -fDensityToHU");
    }
    ReadConversion(densityToHUFN);
    map<G4double,G4int>::iterator ite1 = theDensityToHU.begin(); 
    G4double minDens = ite1->first;
    ite1 = theDensityToHU.end(); ite1--;  
    G4double maxDens = ite1->first;
#ifndef GAMOS_NO_VERBOSE
    if( DicomVerb(infoVerb) ) G4cout << theDensityToHU.size() << " MIN/MAX DENSITIES " << minDens << " < " << maxDens << G4endl; 
    if( DicomVerb(debugVerb) ) {
      for( map<G4double,G4int>::iterator ited = theDensityToHU.begin(); ited != theDensityToHU.end(); ited++ ){
	G4cout << " DENSITY " << ited->first << " = " << ited->second << G4endl; //GDEB
      }
    }
#endif
    
    theInputImage = new DicomVImage(*g4dcmCTImage);
    std::vector<G4double>* voxDataP = g4dcmCTImage->GetData();
    G4double* voxData = &(voxDataP->at(0));
    size_t nVoxels = theInputImage->GetNoVoxels();
    std::vector<G4double>* voxDataP_I = new std::vector<G4double>(nVoxels);
    G4double* voxData_I = &(voxDataP_I->at(0));
    theInputImage->SetData(voxDataP_I);
    voxData = &(voxDataP->at(0));
    for( size_t ii = 0; ii < nVoxels; voxData++, voxData_I++, ii++ ) {
      G4double dens = *voxData;
      //  G4cout << ii << " @@@@ DENS " << dens << " = " << voxDataP->at(ii) << G4endl; //GDEB
      if( dens < minDens || dens > maxDens ) {
	G4Exception("DCMGamosToDICOM::ConvertImages",
		    "",
		    FatalException,
		    ("Density "+GmGenUtils::ftoa(dens)+" is out of conversion limits "+GmGenUtils::ftoa(minDens)+" : "+GmGenUtils::ftoa(maxDens)).c_str());
      }
      map<G4double,G4int>::iterator ite2 = theDensityToHU.lower_bound(dens);
      ite1 = ite2; ite1--;
      //      std::cout <<theDensityToHU.size() <<  " iteHU " << dens << " : " << std::distance(theDensityToHU.begin(),ite1) <<  " : " << std::distance(theDensityToHU.begin(),ite2) << G4endl; //GDEB
      //      std::cout << " HU: " << ite1->first << " " << ite1->second << " : " << ite2->first << " " << ite2->second << G4endl; //GDEB
      G4double HU = ite1->second+(dens - ite1->first)*(ite2->second - ite1->second)/(ite2->first - ite1->first);
      //   G4cout << ii << " HU " << HU << " dens " << dens << G4endl; //GDEB
      //      G4cout << ii << " HU " << HU << " dens " << dens << " = " <<ite1->second << "+"<<(dens - ite1->first)<<"*"<< (ite2->second - ite1->second)<<"/"<<(ite2->first - ite1->first) << G4endl; //GDEB
      //      *voxData_I = int(HU);
      voxDataP_I->at(ii) = int(HU);
      theSmallestValue = min(theSmallestValue,HU);
      theBiggestValue = max(theBiggestValue,HU);
    }				
    modality = DIM_CT;
    bSaveEachZSlice = true;
  }

  imgs = theDicomMgr->GetImages(DIM_G4dcmNM,false);
  if( imgs.size() == 1 ) {
    theInputImage = imgs[0];
    modality = DIM_NM;
    bSaveEachZSlice = false;
  }

  imgs = theDicomMgr->GetImages(DIM_G4dcmDose,false);
  if( imgs.size() == 1 ) {
    theInputImage = imgs[0];
    modality = DIM_RTDose;
    bSaveEachZSlice = false;
  }

  imgs = theDicomMgr->GetImages(DIM_Sqdose,false);
  //  G4cout << " NIMAGES Sqdose " << imgs.size() << G4endl; //GDEB
  if( imgs.size() == 1 ) {
    theInputImage = imgs[0];
    modality = DIM_RTDose;
    bSaveEachZSlice = false;
  }

  imgs = theDicomMgr->GetImages(DIM_Interfile,false);
  //  G4cout << " NIMAGES Interfile " << imgs.size() << G4endl; //GDEB
  if( imgs.size() == 1 ) {
    theInputImage = imgs[0];
    modality = DIM_RTDose;
    bSaveEachZSlice = false;
  }

  bSaveEachZSlice = G4bool(theParamMgr->GetNumericValue("saveEachZSlice",bSaveEachZSlice));
  
#ifndef GAMOS_NO_VERBOSE
  if( DicomVerb(infoVerb) ) G4cout << " INPUT IMAGE " << theInputImage << " MOD " <<  DicomVImage::GetModalityStr(modality) << " bSaveEachZSlice " << bSaveEachZSlice << G4endl;
#endif
  size_t nVoxelZ = theInputImage->GetNoVoxelsZ();
  if( !bSaveEachZSlice ) {
    fillImageToCT(theInputImage, dataset, modality, 0, nVoxelZ-1 );
    // CAN FILL DATASET DIRECTLY, WITHOUT DRTDoseIOD ???
    theOutputFN += theSuffix;
#ifndef GAMOS_NO_VERBOSE
    if( DicomVerb(infoVerb) )   G4cout << "@@@@@@@ SAVING DICOM FILE " << theOutputFN << G4endl;
#endif
    DcmFileFormat fileformat( dataset );
    OFCondition status = fileformat.saveFile(OFFilename(theOutputFN),EXS_LittleEndianExplicit);
    if (status.bad()) {
      std::cerr << "Error: cannot save DICOM file (" << status.text() << ")" << std::endl;
    }
  } else {
    float minZ = theInputImage->GetMinZ();
    float widthZ = theInputImage->GetVoxelDimZ();
    for( size_t iz = 0; iz < nVoxelZ-1; iz++ ) {
      fillImageToCT(theInputImage, dataset, modality, iz, iz);
      // CAN FILL DATASET DIRECTLY, WITHOUT DRTDoseIOD ???
      //      G4String outputFN = theOutputFN+"_"+GmGenUtils::itoa(iz);
      G4String outputFN = theOutputFN+"_"+GmGenUtils::ftoa(minZ+(iz+0.5)*widthZ)+theSuffix;
#ifndef GAMOS_NO_VERBOSE
      if( DicomVerb(infoVerb) ) G4cout << "@@@@@@@ SAVING DICOM FILE " << outputFN << G4endl;
#endif
      DcmFileFormat fileformat( dataset );
      OFCondition status = fileformat.saveFile(OFFilename(outputFN),EXS_LittleEndianExplicit);
      if (status.bad()) {
	std::cerr << "Error: cannot save DICOM file (" << status.text() << ")" << std::endl;
      }      
    }
  }
}

//---------------------------------------------------------------------------
void DCMGamosToDICOM::fillImageToCT( DicomVImage* image, DcmDataset* dataset, DIModality modality, G4int iFirstSlice, G4int iLastSlice )
{
#ifndef GAMOS_NO_VERBOSE
  if( DicomVerb(infoVerb) ) G4cout << " DCMGamosToDICOM::fillImageToCT( " << image->GetName() << " MOD " << modality << " " << DicomVImage::GetModalityStr(modality) << G4endl;
#endif
  
  Uint16 theNBits = 16;
  size_t nVoxelX = image->GetNoVoxelsX();
  size_t nVoxelY = image->GetNoVoxelsY();
  size_t nVoxelZ = (iLastSlice-iFirstSlice)+1;
  size_t nVoxelXY = nVoxelX*nVoxelY;
  float minX = image->GetMinX();
  float minY = image->GetMinY();
  float minZ = image->GetMinZ();
  float widthX = image->GetVoxelDimX();
  float widthY = image->GetVoxelDimY();
  float widthZ = image->GetVoxelDimZ();
  
  G4String date;
  G4String time;
  GmGenUtils::DateAndTimeNow( date, time );
  
  OFCondition result;
  G4String myUID = "1.1.1.1.1.1.1111.111.1.1.11111111111111111111111111111111111111";
  
  DcmUniqueIdentifier InstanceCreatorUID(DCM_InstanceCreatorUID);
  if( bUIDfromRT && theDicomUIDInfo != 0 ) {
    InstanceCreatorUID.putString(theDicomUIDInfo->GetInfo(DCM_InstanceCreatorUID));
  } else {
    InstanceCreatorUID.putString((myUID+"5").c_str());
  }
  addElementToDataset(result, dataset, new DcmUniqueIdentifier(InstanceCreatorUID), "1", "1", "SOPCommonModule");
  
  // --- PatientModule (M) ---
  addElementToDataset(result, dataset, new DcmPersonName(DCM_PatientName), "1", "2", "PatientModule");
  addElementToDataset(result, dataset, new DcmLongString(DCM_PatientID), "1", "2", "PatientModule");
  //  if (result.good()) result = IssuerOfPatientIDQualifiersSequence.write(dataset, "1-n" ,"3", "PatientModule");
  addElementToDataset(result, dataset, new DcmDate(DCM_PatientBirthDate), "1", "2", "PatientModule");
  addElementToDataset(result, dataset, new DcmCodeString(DCM_PatientSex), "1", "2", "PatientModule");
  //  if (result.good()) result = ReferencedPatientPhotoSequence.write(dataset, "1-n" ,"3", "PatientModule");
  // addElementToDataset(result, dataset, new DcmCodeString(QualityControlSubject), "1", "3", "PatientModule");
  //  if (result.good()) result = ReferencedPatientSequence.write(dataset, "1-n" ,"3", "PatientModule");
  addElementToDataset(result, dataset, new DcmTime(DCM_PatientBirthTime), "1", "3", "PatientModule");
  
  // --- GeneralStudyModule (M) ---
  DcmUniqueIdentifier StudyInstanceUID(DCM_StudyInstanceUID);
  if( theDicomUIDInfo != 0 ) {
    StudyInstanceUID.putString(theDicomUIDInfo->GetInfo(DCM_StudyInstanceUID));
  } else {
    StudyInstanceUID.putString((myUID+"1").c_str());
  }
  addElementToDataset(result, dataset, new DcmUniqueIdentifier(StudyInstanceUID), "1", "1", "GeneralStudyModule");
  DcmDate StudyDate(DCM_StudyDate); StudyDate.putString(date);
  addElementToDataset(result, dataset, new DcmDate(StudyDate), "1", "2", "GeneralStudyModule");
  DcmTime StudyTime(DCM_StudyTime); StudyTime.putString(time);
  addElementToDataset(result, dataset, new DcmTime(StudyTime), "1", "2", "GeneralStudyModule");
  addElementToDataset(result, dataset, new DcmPersonName(DCM_ReferringPhysicianName), "1", "2", "GeneralStudyModule");
  addElementToDataset(result, dataset, new DcmShortString(DCM_StudyID), "1", "2", "GeneralStudyModule");
  addElementToDataset(result, dataset, new DcmShortString(DCM_AccessionNumber), "1", "2", "GeneralStudyModule");
  DcmLongString StudyDescription(DCM_StudyDescription); StudyDescription.putString("GAMOS");
  addElementToDataset(result, dataset, new DcmLongString(StudyDescription), "1", "3", "GeneralStudyModule");
  
  // --- PatientStudyModule (U) ---
  // --- ClinicalTrialSubjectModule (U) ---
  
  // --- RTSeriesModule (M) ---
  G4String modD = DicomVImage::GetModalityStr(modality).substr(4,99);
  DcmCodeString Modality(DCM_Modality); Modality.putString(modD);
  addElementToDataset(result, dataset, new DcmCodeString(Modality), "1", "1", "RTSeriesModule");
  DcmUniqueIdentifier SeriesInstanceUID(DCM_SeriesInstanceUID);
  if( bUIDfromRT && theDicomUIDInfo != 0 ) {
    SeriesInstanceUID.putString(theDicomUIDInfo->GetInfo(DCM_SeriesInstanceUID));
  } else {
    SeriesInstanceUID.putString((myUID+"2").c_str());
  }
  addElementToDataset(result, dataset, new DcmUniqueIdentifier(SeriesInstanceUID), "1", "1", "RTSeriesModule");
  DcmIntegerString SeriesNumber(DCM_SeriesNumber); SeriesNumber.putOFStringArray("1");
  addElementToDataset(result, dataset, new DcmIntegerString(SeriesNumber), "1", "2", "RTSeriesModule");
  
  // --- GeneralSeriesModule (M) ---
  //  thePatientPosition ="HFS";
  DcmCodeString PatientPosition(DCM_PatientPosition); PatientPosition.putString(thePatientPosition.c_str());
  //DcmCodeString PatientPosition(DCM_PatientPosition); PatientPosition.putString("HFS");
  addElementToDataset(result, dataset, new DcmCodeString(PatientPosition), "1", "1", "GeneralSeriesModule");
  
  // --- ClinicalTrialSeriesModule (U) ---
  // --- FrameOfReferenceModule (M) ---
  DcmUniqueIdentifier FrameOfReferenceUID(DCM_FrameOfReferenceUID); 
  if( bUIDfromRT && theDicomUIDInfo != 0 ) {
    FrameOfReferenceUID.putString(theDicomUIDInfo->GetInfo(DCM_FrameOfReferenceUID));
  } else {
    FrameOfReferenceUID.putString((myUID+"3").c_str());
  }
  addElementToDataset(result, dataset, new DcmUniqueIdentifier(FrameOfReferenceUID), "1", "1", "FrameOfReferenceModule");
  addElementToDataset(result, dataset, new DcmLongString(DCM_PositionReferenceIndicator), "1", "2", "FrameOfReferenceModule");
  
  // --- GeneralEquipmentModule (M) ---
  DcmLongString ManufacturerModelName(DCM_ManufacturerModelName); ManufacturerModelName.putString("GAMOS");
  addElementToDataset(result, dataset, new DcmLongString(ManufacturerModelName), "1", "3", "GeneralEquipmentModule");
  DcmLongString SoftwareVersions(DCM_SoftwareVersions); SoftwareVersions.putString(getenv("GAMOS_VER"));
  addElementToDataset(result, dataset, new DcmLongString(SoftwareVersions), "1-n", "3", "GeneralEquipmentModule");
  
  // --- GeneralImageModule (C) ---
  // --- ImagePlaneModule (C) ---
  OFString value;
  value.append("2"); value.append("3");
  //  std::cout << " STRING OK " << (DcmDecimalString::checkStringValue(value, "2")).good() << std::endl; //GDEB
  double PixelWidth = widthX;
  double PixelHeight = widthY;
  char buffer[32];
  OFStandard::ftoa(buffer, 15, PixelWidth, OFStandard::ftoa_format_f);
  strcat(buffer, "\\");
  OFStandard::ftoa(strchr(buffer, 0), 15, PixelHeight, OFStandard::ftoa_format_f);
  DcmDecimalString PixelSpacing(DCM_PixelSpacing); PixelSpacing.putString(buffer);
  addElementToDataset(result, dataset, new DcmDecimalString(PixelSpacing), "2", "1", "ImagePlaneModule");
  G4String imageOrient = "1\\0\\0\\0\\1\\0";
  DcmDecimalString SliceThickness(DCM_SliceThickness); SliceThickness.putString(GmGenUtils::ftoa(widthZ));
  addElementToDataset(result, dataset, new DcmDecimalString(SliceThickness), "1", "2", "ImagePlaneModule");
  DcmDecimalString ImageOrientationPatient(DCM_ImageOrientationPatient); ImageOrientationPatient.putString(imageOrient);
  addElementToDataset(result, dataset, new DcmDecimalString(ImageOrientationPatient), "6", "1", "ImagePlaneModule");
  
  // imagePositionPatient: hadd 1/2 voxel size
  G4double imagePosPatX = minX + 0.5*PixelWidth;
  G4double imagePosPatY = minY + 0.5*PixelHeight;
  G4double imagePosPatZ = minZ + (iFirstSlice+0.5)*widthZ;
  G4String imagePos = GmGenUtils::ftoa(imagePosPatX) + "\\" +
    GmGenUtils::ftoa(imagePosPatY) + "\\" +
    GmGenUtils::ftoa(imagePosPatZ);  
  
  DcmDecimalString ImagePositionPatient(DCM_ImagePositionPatient); ImagePositionPatient.putString(imagePos);
  addElementToDataset(result, dataset, new DcmDecimalString(ImagePositionPatient), "3", "1", "ImagePlaneModule");

  if( nVoxelZ == 1 ) {
    G4double sliceLocation = minZ + (iFirstSlice+0.5)*widthZ;
    DcmDecimalString SliceLocation(DCM_SliceLocation); SliceLocation.putString(GmGenUtils::ftoa(sliceLocation));
    addElementToDataset(result, dataset, new DcmDecimalString(SliceLocation), "1", "3", "ImagePlaneModule");
    G4cout << " SLICE_LOCATION " << sliceLocation << G4endl; //GDEB
  }
  
  // --- ImagePixelModule (C) ---
  DcmUnsignedShort Rows(DCM_Rows); Rows.putUint16(nVoxelY);
  addElementToDataset(result, dataset, new DcmUnsignedShort(Rows), "1", "1", "ImagePixelModule");
  DcmUnsignedShort Columns(DCM_Columns); Columns.putUint16(nVoxelX);
  addElementToDataset(result, dataset, new DcmUnsignedShort(Columns), "1", "1", "ImagePixelModule");
  
  unsigned long nVoxels = nVoxelX*nVoxelY*nVoxelZ;
  Uint16* pd = new Uint16[nVoxels];
  std::vector<G4double>* voxDataP = image->GetData();
  //--- NORMALIZE TO MAXIMUM
  G4double maxData = 0;
  G4double* voxData = &(voxDataP->at(0));
  //  G4cout << nVoxels << " 0data " << voxData << " N " << voxDataP->size() <<  G4endl; //GDEB

  voxData += nVoxelXY*iFirstSlice;
  for( size_t ii = 0; ii < nVoxels; voxData++,ii++ ) {
    //    G4cout << ii << " voxData " << voxData << G4endl; //GDEB
    maxData = max(maxData,*voxData);
  }
#ifndef GAMOS_NO_VERBOSE
  if( DicomVerb(infoVerb) ) G4cout << " MAXDATA " << maxData << G4endl;
#endif
  G4double factor = 1;
  if( modality == DIM_CT ) { 
    DcmUniqueIdentifier SOPClassUID(DCM_SOPClassUID);
    SOPClassUID.putString(UID_CTImageStorage);
    addElementToDataset(result, dataset, new DcmUniqueIdentifier(SOPClassUID), "1", "1", "SOPCommonModule");
    DcmUnsignedShort PixelRepresentation(DCM_PixelRepresentation); PixelRepresentation.putUint16(1);
    addElementToDataset(result, dataset, new DcmUnsignedShort(PixelRepresentation), "1", "1C", "ImagePixelModule");
    factor = 1;
  } else if( modality == DIM_NM ) {
    DcmUniqueIdentifier SOPClassUID(DCM_SOPClassUID);
    SOPClassUID.putString(UID_NuclearMedicineImageStorage);
    addElementToDataset(result, dataset, new DcmUniqueIdentifier(SOPClassUID), "1", "1", "SOPCommonModule");
    factor = (pow(2,theNBits)-1)/maxData;
    DcmUnsignedShort PixelRepresentation(DCM_PixelRepresentation); PixelRepresentation.putUint16(0);
    addElementToDataset(result, dataset, new DcmUnsignedShort(PixelRepresentation), "1", "1C", "ImagePixelModule");
  } else if( modality == DIM_RTDose ) {
    DcmUniqueIdentifier SOPClassUID(DCM_SOPClassUID);
    SOPClassUID.putString(UID_RTDoseStorage);
    addElementToDataset(result, dataset, new DcmUniqueIdentifier(SOPClassUID), "1", "1", "SOPCommonModule");
    factor = (pow(2,theNBits)-1)/maxData;
    DcmUnsignedShort PixelRepresentation(DCM_PixelRepresentation); PixelRepresentation.putUint16(0);
    addElementToDataset(result, dataset, new DcmUnsignedShort(PixelRepresentation), "1", "1C", "ImagePixelModule");
    DcmDecimalString DoseGridScaling(DCM_DoseGridScaling); DoseGridScaling.putString(GmGenUtils::ftoa(1./factor));
    addElementToDataset(result, dataset, new DcmDecimalString(DoseGridScaling), "1", "1C", "RTDoseModule");
    DcmDecimalString DoseUnits(DCM_DoseUnits); DoseUnits.putString("GY");
    addElementToDataset(result, dataset, new DcmDecimalString(DoseUnits), "1", "1C", "RTDoseModule");
    DcmDecimalString DoseType(DCM_DoseType); DoseType.putString("MONTECARLO");
    addElementToDataset(result, dataset, new DcmDecimalString(DoseType), "1", "1C", "RTDoseModule");
    DcmDecimalString DoseSummationType(DCM_DoseSummationType); DoseSummationType.putString("BEAM");
    addElementToDataset(result, dataset, new DcmDecimalString(DoseSummationType), "1", "1C", "RTDoseModule");
    if( nVoxelZ > 1 ) {
      G4String gridFrameOffset;
      for( size_t iz = 0; iz < nVoxelZ; iz++ ) {
	G4double posz = iz*widthZ;
	if( iz != 0 ) gridFrameOffset += "\\";
	gridFrameOffset += GmGenUtils::ftoa(posz);
      }
      DcmDecimalString GridFrameOffsetVector(DCM_GridFrameOffsetVector); GridFrameOffsetVector.putString(gridFrameOffset);
      addElementToDataset(result, dataset, new DcmDecimalString(GridFrameOffsetVector), "1", "1C", "RTDoseModule");
    }

  }
  
  voxDataP = image->GetData();
  voxData = &(voxDataP->at(0));
  voxData += nVoxelXY*iFirstSlice;
  for( size_t ii = 0; ii < nVoxels; ii++ ) {
    size_t ix = ii%nVoxelX;
    size_t iy = (ii/nVoxelX)%nVoxelY;
    size_t iz = ii/nVoxelXY;
    size_t iiInvZ = ix + iy*nVoxelX + iz*nVoxelXY;
    if(imagePos == "FFS" ) {
    // invert Z axis !!!
      iiInvZ = ix + iy*nVoxelX + (nVoxelZ-iz-1)*nVoxelXY;
    }
    pd[ii] = Uint16(voxData[iiInvZ]*factor+1);
#ifndef GAMOS_NO_VERBOSE
    if( DicomVerb(infoVerb) && ii%100000 == 0 ) G4cout << ii << "=" << ix<<":"<<iy<<":"<<iz << " PIXEL_DATA " << pd[ii] << " = " << voxData[iiInvZ] *factor << " <- " << *voxData << " factor " << factor << G4endl; //GDEB
    //    if(  doses[iiInvZ] != 0 ) G4cout << ii << " -> " << iiInvZ << " DOSE " << pd[ii] << " = " <<doses[iiInvZ]*factor << " <- " <<  doses[iiInvZ] << G4endl; //GDEB
#endif
  }
  DcmPixelData PixelData(DCM_PixelData); PixelData.putUint16Array (pd,nVoxels);
  //utUint16Array (c(const Uint16 *wordValue, const unsigned long length);
  addElementToDataset(result, dataset, new DcmPixelData(PixelData), "1", "1C", "ImagePixelModule");

  DcmUnsignedShort SmallestImagePixelValue(DCM_SmallestImagePixelValue); SmallestImagePixelValue.putUint16(0);
  addElementToDataset(result, dataset, new DcmUnsignedShort(SmallestImagePixelValue), "1", "3", "ImagePixelModule");
  DcmUnsignedShort LargestImagePixelValue(DCM_LargestImagePixelValue); LargestImagePixelValue.putUint16(pow(2,theNBits)-1);
  addElementToDataset(result, dataset, new DcmUnsignedShort(LargestImagePixelValue), "1", "3", "ImagePixelModule");

  // --- MultiFrameModule (C) ---
  DcmIntegerString NumberOfFrames(DCM_NumberOfFrames); NumberOfFrames.putString(GmGenUtils::itoa(nVoxelZ));
  addElementToDataset(result, dataset, new DcmIntegerString(NumberOfFrames), "1", "1", "MultiFrameModule");

  DcmUnsignedShort NumberOfSlices(DCM_NumberOfSlices); NumberOfSlices.putUint16(nVoxelZ);
  addElementToDataset(result, dataset, new DcmUnsignedShort(NumberOfSlices), "1", "1C", "ImagePixelModule");

  //  addElementToDataset(result, dataset, new DcmAttributeTag(FrameIncrementPointer), "1-n", "1", "MultiFrameModule");

  DcmUnsignedShort SamplesPerPixel(DCM_SamplesPerPixel); SamplesPerPixel.putUint16(1);
  addElementToDataset(result, dataset, new DcmUnsignedShort(SamplesPerPixel), "1", "1C", "ImagePixelModule");
  DcmCodeString PhotometricInterpretation(DCM_PhotometricInterpretation); PhotometricInterpretation.putString("MONOCHROME2"); 
  addElementToDataset(result, dataset, new DcmCodeString(PhotometricInterpretation), "1", "1C", "ImagePixelModule");
  DcmUnsignedShort BitsAllocated(DCM_BitsAllocated); BitsAllocated.putUint16(theNBits);
  addElementToDataset(result, dataset, new DcmUnsignedShort(BitsAllocated), "1", "1C", "ImagePixelModule");
  DcmUnsignedShort BitsStored(DCM_BitsStored); BitsStored.putUint16(theNBits);
  addElementToDataset(result, dataset, new DcmUnsignedShort(BitsStored), "1", "1C", "ImagePixelModule");
  DcmUnsignedShort HighBit(DCM_HighBit); HighBit.putUint16(theNBits-1);
  addElementToDataset(result, dataset, new DcmUnsignedShort(HighBit), "1", "1C", "ImagePixelModule");
  DcmIntegerString InstanceNumber(DCM_InstanceNumber); InstanceNumber.putString("1");
  addElementToDataset(result, dataset, new DcmIntegerString(InstanceNumber), "1", "3", "ImagePixelModule");

  // --- RTDVHModule (U) ---
  // --- StructureSetModule (C) ---
  // --- ROIContourModule (C) ---
  // --- RTDoseROIModule (C) ---

  // --- SOPCommonModule (M) ---  
  DcmUniqueIdentifier SOPInstanceUID(DCM_SOPInstanceUID);
  if( bUIDfromRT && theDicomUIDInfo != 0 ) {
    SOPInstanceUID.putString(theDicomUIDInfo->GetInfo(DCM_SOPInstanceUID));
  } else {
    SOPInstanceUID.putString((myUID+"4").c_str());
  }
  addElementToDataset(result, dataset, new DcmUniqueIdentifier(SOPInstanceUID), "1", "1", "SOPCommonModule");
  
  DcmCodeString SpecificCharacterSet(DCM_SpecificCharacterSet); SpecificCharacterSet.putString("ISO_IR 100");  //Latin alphabet No. 1
  addElementToDataset(result, dataset, new DcmCodeString(SpecificCharacterSet), "1-n", "1C", "SOPCommonModule");
  DcmDate InstanceCreationDate(DCM_InstanceCreationDate); InstanceCreationDate.putString(date);
  addElementToDataset(result, dataset, new DcmDate(InstanceCreationDate), "1", "3", "SOPCommonModule");
  DcmTime InstanceCreationTime(DCM_InstanceCreationTime); InstanceCreationTime.putString(time);
  addElementToDataset(result, dataset, new DcmTime(InstanceCreationTime), "1", "3", "SOPCommonModule");
  
  // --- CommonInstanceReferenceModule (U) ---
  // --- FrameExtractionModule (C) ---
  
  }
  

OFCondition DCMGamosToDICOM::addElementToDataset(OFCondition &result,
					   DcmItem *dataset,
					   DcmElement *element,
					   const OFString &vm,
					   const OFString &type,
					   const char *moduleName)
{
  if (element != NULL) {
    OFBool triedToInsert = OFFalse;
    if (result.good()) {
      if ((type == "2") || !element->isEmpty())  {
	triedToInsert = OFTrue;
	/* insert non-empty element or empty "type 2" element */
	result = dataset->insert(element, OFTrue /*replaceOld*/);
	if (DCM_dcmrtLogger.isEnabledFor(OFLogger::WARN_LOG_LEVEL))
	  checkElementValue(*element, vm, type, result, moduleName);
      } else if (type == "1") {
	/* empty element value not allowed for "type 1" */
	result = RT_EC_InvalidValue;
	if (DCM_dcmrtLogger.isEnabledFor(OFLogger::WARN_LOG_LEVEL))
	  checkElementValue(*element, vm, type, result, moduleName);
      }
    }
    /* delete element if not inserted into the dataset */
    if (result.bad() || !triedToInsert) delete element;
  } else {
    result = EC_MemoryExhausted;
  }
  return result;
}

OFBool DCMGamosToDICOM::checkElementValue(DcmElement &element,
				   const OFString &vm,
				   const OFString &type,
				   const OFCondition &searchCond,
				   const char *moduleName)
{
  OFBool result = OFTrue;
  DcmTag tag = element.getTag();
  const OFString tagName = tag.getTagName();
  const OFString module = (moduleName == NULL) ? "RT object" : moduleName;
  /* NB: type 1C and 2C cannot be checked, assuming to be optional = type 3 */
  if (((type == "1") || (type == "2")) && searchCond.bad())
    {
      DCMRT_WARN(tagName << " " << tag << " absent in " << module << " (type " << type << ")");
      result = OFFalse;
    }
  else if (element.isEmpty(OFTrue /*normalize*/))
    {
      /* however, type 1C should never be present with empty value */
      if (((type == "1") || (type == "1C")) && searchCond.good())
	{
	  DCMRT_WARN(tagName << " " << tag << " empty in " << module << " (type " << type << ")");
	  result = OFFalse;
	}
    } else {
    const OFCondition checkResult = element.checkValue(vm, OFTrue /*oldFormat*/);
    if (checkResult == EC_ValueRepresentationViolated)
      {
	DCMRT_WARN(tagName << " " << tag << " violates VR definition in " << module);
	result = OFFalse;
      }
    else if (checkResult == EC_ValueMultiplicityViolated)
      {
	const OFString vmText = (element.getVR() == EVR_SQ) ? " #items" : " VM";
	DCMRT_WARN(tagName << " " << tag << vmText << " != " << vm << " in " << module);
	result = OFFalse;
      }
    else if (checkResult == EC_MaximumLengthViolated)
      {
	DCMRT_WARN(tagName << " " << tag << " violates maximum VR length in " << module);
	result = OFFalse;
      }
    else if (checkResult.bad())
      {
	DCMRT_DEBUG("INTERNAL ERROR while checking value of " << tagName << " " << tag << " in " << module);
      }
  }
  return result;
}


//-----------------------------------------------------------------------------
void DCMGamosToDICOM::ReadConversion( G4String fileName )
{
  std::vector<G4String> wl; 
  //----- Open file
  GmFileIn fin = GmFileIn::GetInstance(fileName, true);
  //  if( DicomVerb(warningVerb) ) G4cout << "@@@@@@@ Reading FILE: " << fileName << G4endl;
  //----- Read metadata 
  int ii;
  G4double prevDensity = -1;
  for( ii = 0;; ii++) {
    if( ! fin.GetWordsInLine(wl) ) break;
    if( wl[0] == ":CT2D" ) {
      if( wl.size() != 3 ){
	G4Exception("Dicom2G4FileMgr::Convert",
		    "",
		    FatalErrorInArgument,
		    ":CT2D lines must have three words: :CT2D HU Density");
      }
      G4double density =  GmGenUtils::GetValue(wl[2]);
      if( density <= prevDensity ) {
	G4cerr << " density= " << density << " <=? prevDensity " << prevDensity << G4endl;
	G4Exception("DCMGamosToDICOM::ReadConversion",
		    "",
		    FatalException,
		    "Densities must be in increasing order");
      }      
      theDensityToHU[density] = GmGenUtils::GetInt(wl[1]);
      prevDensity = density;
      G4cout << " AddDensityToHU " << theDensityToHU.size() << " : " << GmGenUtils::GetInt(wl[1]) << " = " << GmGenUtils::GetValue(wl[2]) <<G4endl;
    }
  }
}

//-----------------------------------------------------------------------------
void DCMGamosToDICOM::PrintHelp()
{
  ((DicomParameterMgr*)(DicomParameterMgr::GetInstance()))->PrintHelpInputFiles();
  G4cout << " -patientPosition   PatientPosition (default is HFS)" << G4endl
	 << " -fDensityToHU      File containing the density to Hounsfield number conversion" << G4endl
	 << " -suffix            Suffix for output files" << G4endl
	 << " -saveEachZSlice    Save a file per each Z sice (default is dcm) " << G4endl; 
}
  

