#include "DCMSqToRT.hh"
#include "DICOM/DICOMBase/include/DicomVerbosity.hh"
#include "DICOM/DICOMBase/include/DicomParameterMgr.hh"
#include "DICOM/DICOMReaders/include/DicomReaderMgr.hh"
#include "DICOM/DICOMReaders/include/DicomUIDInfo.hh"

#include "GamosCore/GamosReadDICOM/include/GmSqdose.hh"
#include "GamosCore/GamosReadDICOM/include/GmSqdoseHeader.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

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

//------------------------------------------------------------------------
DCMSqToRT::DCMSqToRT()
{
  theParamMgr = (DicomParameterMgr*)(DicomParameterMgr::GetInstance());
  theReaderMgr = DicomReaderMgr::GetInstance();
}

//------------------------------------------------------------------------
void DCMSqToRT::ProcessArguments( int argc, char** argv ) 
{
  theExeName = "sqdoseToRTDose";
  theSqDoseFN = "";
  theRTDoseFN = "";
  thePatientPosition = "HFS";
  
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
  } else if( argc == 3 ) {    
    theSqDoseFN = argv[1];
    theRTDoseFN = argv[2];
    if( theSqDoseFN[0] == '-' || theRTDoseFN[0] == '-' ) {
      G4Exception(theExeName,
		  "",
		  FatalErrorInArgument,
		  "TWO ARGUMENTS MUST BE SQDOSE_INPUT_FILE RTDOSE_OUTPUT_FILE");
    }
    G4cout << " theSqDoseFN " << theSqDoseFN << " theRTDoseFN " << theRTDoseFN << " thePatientPosition " << thePatientPosition << G4endl;
    return;
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
      if( argvstr == "-fSqdoseIn" ) {
	theParamMgr->AddParam( argvName + " " + argvstr1, PTstring );
	ii++;
      } else if( argvstr == "-fRTDoseOut" ) {
	theParamMgr->AddParam( argvName + " " + argvstr1, PTstring );
	ii++;
      } else if( argvstr == "-patientPosition" ) {
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
  theSqDoseFN = theParamMgr->GetStringValue("fSqdoseIn",theSqDoseFN);
  theRTDoseFN = theParamMgr->GetStringValue("fRTDoseOut",theRTDoseFN);
  thePatientPosition = theParamMgr->GetStringValue("patientPosition","HFS");
  G4cout << " theSqDoseFN " << theSqDoseFN << " theRTDoseFN " << theRTDoseFN << " thePatientPosition " << thePatientPosition << G4endl;
}

//---------------------------------------------------------------------------
void DCMSqToRT::CheckArguments()
{
  //------ At least one image file!
  if( theSqDoseFN == "" ) {
    PrintHelp();
    G4Exception(theExeName,
		"No SqDose input file",
		FatalException,
		"Use -fSqdoseIn in command line");
  }
  if( theRTDoseFN == "" ) {
    PrintHelp();
    G4Exception(theExeName,
		"No RTDoseFN output file",
		FatalException,
		"Use -fRTDoseOut in command line");
  }

  // OPTIONALLY ONE DICOM FILE TO SET UID VALUES

  //----- CHECK CORRECT ARGUMENTS
  if( theParamMgr->GetNofImageFiles("fG4dcmCT") != 0 ) {
    G4Exception("DCMSqToRT::CheckArguments","",FatalException,"THERE CANNOT BE ANY IMAGE FILE OF TYPE G4dcmCT");
  }
  if( theParamMgr->GetNofImageFiles("fG4dcmNM") != 0 ) {
    G4Exception("DCMSqToRT::CheckArguments","",FatalException,"THERE CANNOT BE ANY IMAGE FILE OF TYPE G4dcmNM");
  }
  if( theParamMgr->GetNofImageFiles("fG4dcmDose") != 0 ) {
    G4Exception("DCMSqToRT::CheckArguments","",FatalException,"THERE CANNOT BE ANY IMAGE FILE OF TYPE G4dcmDose");
  }
  if( theParamMgr->GetNofImageFiles("fSqdose") != 0 ) {
    G4Exception("DCMSqToRT::CheckArguments","",FatalException,"THERE CANNOT BE ANY '-fSqdose' PARAMETER, PLEASE USE '-fSqdoseIn'");
  }
  if( theParamMgr->GetNofImageFiles("fRTStruct") > 1 ) {
    G4Exception("DCMSqToRT::CheckArguments","",FatalException,"THERE CANNOT BE ANY IMAGE FILE OF TYPE RTSTRUCT");
  }
  G4int ndcm = theParamMgr->GetNofImageFiles("fCT");
  ndcm += theParamMgr->GetNofImageFiles("fNM");
  ndcm += theParamMgr->GetNofImageFiles("fRTDOSE");
  if( ndcm > 1 ) {
    G4Exception("DCMSqToRT::CheckArguments","",FatalException,"THERE CAN ONLY BE ONE IMAGE OF TYPES CT, NM AND RTDOSE");
  }
  
}

//---------------------------------------------------------------------------
void DCMSqToRT::ConvertImages()
{
  //-----  Get DicomUIDInfo from one of the DICOM files
  //--- READ DICOM FILES FOR UID
  theReaderMgr->ProcessData();
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
  //--------- READ Sqdose
  GmSqdose* sqdose = 0; 
  if( DicomVerb(infoVerb) ) G4cout << "@@@@@@@ READING SQDOSE FILE: " << theSqDoseFN << G4endl;
  sqdose = new GmSqdose();
  sqdose->Read(theSqDoseFN);
  //    sqdose->CalculateErrors();

  DcmDataset* dataset = new DcmDataset();
  //DRTDoseIOD rtdose;
  fillRTDoseFromSqdose( sqdose, *dataset );

  // CAN FILL DATASET DIRECTLY, WITHOUT DRTDoseIOD ???
  DcmFileFormat fileformat( dataset );
  if( DicomVerb(infoVerb) )   G4cout << "@@@@@@@ SAVING RTDOSE FILE " << theRTDoseFN << G4endl;

  OFCondition status = fileformat.saveFile(OFFilename(theRTDoseFN),EXS_LittleEndianExplicit);
  if (status.bad()) {
    std::cerr << "Error: cannot save DICOM file (" << status.text() << ")" << std::endl;
  }
}

void DCMSqToRT::fillRTDoseFromSqdose( GmSqdose* sqdose, DcmDataset& dataset )
{
  Uint16 theNBits = 16;
  GmSqdoseHeader* sqdoseHeader = sqdose->GetHeader();
  size_t nVoxelX = sqdoseHeader->GetNoVoxelsX();
  size_t nVoxelY = sqdoseHeader->GetNoVoxelsY();
  size_t nVoxelZ = sqdoseHeader->GetNoVoxelsZ();
  std::vector<float> voxelLimitsX = sqdoseHeader->GetVoxelLimitsX();
  std::vector<float> voxelLimitsY = sqdoseHeader->GetVoxelLimitsY();
  std::vector<float> voxelLimitsZ = sqdoseHeader->GetVoxelLimitsZ();
  float widthX = sqdoseHeader->GetVoxelHalfX()*2;
  float widthY = sqdoseHeader->GetVoxelHalfY()*2;
  float widthZ = sqdoseHeader->GetVoxelHalfZ()*2;

  G4String date;
  G4String time;
  GmGenUtils::DateAndTimeNow( date, time );

  OFCondition result;
  G4String myUID = "1.1.1.1.1.1.1111.111.1.1.11111111111111111111111111111111111111";

  /*  DcmUniqueIdentifier TransferSyntaxUID(DCM_TransferSyntaxUID);
  if( GmGenUtils::IsLittleEndian() ) {
    TransferSyntaxUID.putString("LittleEndianExplicit");
  } else {
    TransferSyntaxUID.putString("BigEndianExplicit");
  }
  addElementToDataset(result, dataset, new DcmUniqueIdentifier(TransferSyntaxUID), "1", "1", "MetaInformationHeader");

  DcmUniqueIdentifier ImplementationClassUID(DCM_ImplementationClassUID);
  if( bUIDfromRT && theDicomUIDInfo != 0 ) {
    ImplementationClassUID.putString(theDicomUIDInfo->GetInfo(DCM_ImplementationClassUID));
  } else {
    ImplementationClassUID.putString("1.1.11.1.1111.1.1.11111111");
  }
  addElementToDataset(result, dataset, new DcmUniqueIdentifier(ImplementationClassUID), "1", "1", "MetaInformationHeader");
  */
  
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
  DcmCodeString Modality(DCM_Modality); Modality.putString("RTDOSE");
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
  DcmCodeString PatientPosition(DCM_PatientPosition); PatientPosition.putString(thePatientPosition);
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
  DcmDecimalString ImageOrientationPatient(DCM_ImageOrientationPatient); ImageOrientationPatient.putString(imageOrient);
  addElementToDataset(result, dataset, new DcmDecimalString(ImageOrientationPatient), "6", "1", "ImagePlaneModule");
  // imagePositionPatient: hadd 1/2 voxel size
  G4double imagePosPatX = voxelLimitsX[0] + 0.5*PixelWidth;
  G4double imagePosPatY = voxelLimitsY[0] + 0.5*PixelHeight;
  G4double imagePosPatZ = voxelLimitsZ[0];
  G4String imagePos = GmGenUtils::ftoa(imagePosPatX) + "\\" +
    GmGenUtils::ftoa(imagePosPatY) + "\\" +
    GmGenUtils::ftoa(imagePosPatZ);  
  DcmDecimalString ImagePositionPatient(DCM_ImagePositionPatient); ImagePositionPatient.putString(imagePos);
  addElementToDataset(result, dataset, new DcmDecimalString(ImagePositionPatient), "3", "1", "ImagePlaneModule");
  DcmDecimalString SliceThickness(DCM_SliceThickness); SliceThickness.putString(GmGenUtils::ftoa(widthZ));
  addElementToDataset(result, dataset, new DcmDecimalString(SliceThickness), "1", "2", "ImagePlaneModule");
  // addElementToDataset(result, dataset, new DcmDecimalString(SliceLocation), "1", "3", "ImagePlaneModule");

  // --- ImagePixelModule (C) ---
  DcmUnsignedShort Rows(DCM_Rows); Rows.putUint16(nVoxelY);
  addElementToDataset(result, dataset, new DcmUnsignedShort(Rows), "1", "1", "ImagePixelModule");
  DcmUnsignedShort Columns(DCM_Columns); Columns.putUint16(nVoxelX);
  addElementToDataset(result, dataset, new DcmUnsignedShort(Columns), "1", "1", "ImagePixelModule");

  unsigned long nVoxels = nVoxelX*nVoxelY*nVoxelZ;
  Uint16* pd = new Uint16[nVoxels];
  std::vector<float> doses = sqdose->GetDoses();
  //--- NORMALIZE TO MAXIMUM
  float maxDose = 0;
  for( size_t ii = 0; ii < nVoxels; ii++ ) {
    maxDose = std::max(maxDose,doses[ii]);
  }
  //  G4cout << " MAXDOSE " << maxDose << G4endl; //GDEB
  G4double factor = (pow(2,theNBits)-1)/maxDose;
  DcmDecimalString DoseGridScaling(DCM_DoseGridScaling); DoseGridScaling.putString(GmGenUtils::ftoa(1./factor));
  addElementToDataset(result, dataset, new DcmDecimalString(DoseGridScaling), "1", "1C", "RTDoseModule");


  size_t nVoxelXY = nVoxelX*nVoxelY;
  for( size_t ii = 0; ii < nVoxels; ii++ ) {
    // invert Z axis !!!
    size_t ix = ii%nVoxelX;
    size_t iy = (ii/nVoxelX)%nVoxelY;
    size_t iz = ii/nVoxelXY;
    size_t iiInvZ = ix + iy*nVoxelX + iz*nVoxelXY;
    if(imagePos == "FFS" ) {
      iiInvZ = ix + iy*nVoxelX + (nVoxelZ-iz-1)*nVoxelXY;
    }
    pd[ii] = Uint16(doses[iiInvZ]*factor);
    //    if( ii%1000 == 0 ) G4cout << ii << " DOSE " << pd[ii] << " = " <<doses[ii]*factor << " <- " <<  doses[ii] << G4endl; //GDEB
    //    if(  doses[iiInvZ] != 0 ) G4cout << ii << " -> " << iiInvZ << " DOSE " << pd[ii] << " = " <<doses[iiInvZ]*factor << " <- " <<  doses[iiInvZ] << G4endl; //GDEB
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
  
  //  addElementToDataset(result, dataset, new DcmAttributeTag(FrameIncrementPointer), "1-n", "1", "MultiFrameModule");

  // --- RTDoseModule (M) ---
  DcmUnsignedShort SamplesPerPixel(DCM_SamplesPerPixel); SamplesPerPixel.putUint16(1);
  addElementToDataset(result, dataset, new DcmUnsignedShort(SamplesPerPixel), "1", "1C", "RTDoseModule");
  DcmCodeString PhotometricInterpretation(DCM_PhotometricInterpretation); PhotometricInterpretation.putString("MONOCHROME2"); 
  addElementToDataset(result, dataset, new DcmCodeString(PhotometricInterpretation), "1", "1C", "RTDoseModule");
  DcmUnsignedShort BitsAllocated(DCM_BitsAllocated); BitsAllocated.putUint16(theNBits);
  addElementToDataset(result, dataset, new DcmUnsignedShort(BitsAllocated), "1", "1C", "RTDoseModule");
  DcmUnsignedShort BitsStored(DCM_BitsStored); BitsStored.putUint16(theNBits);
  addElementToDataset(result, dataset, new DcmUnsignedShort(BitsStored), "1", "1C", "RTDoseModule");
  DcmUnsignedShort HighBit(DCM_HighBit); HighBit.putUint16(theNBits-1);
  addElementToDataset(result, dataset, new DcmUnsignedShort(HighBit), "1", "1C", "RTDoseModule");
  DcmUnsignedShort PixelRepresentation(DCM_PixelRepresentation); PixelRepresentation.putUint16(0);
  addElementToDataset(result, dataset, new DcmUnsignedShort(PixelRepresentation), "1", "1C", "RTDoseModule");
  DcmCodeString DoseUnits(DCM_DoseUnits); DoseUnits.putString("GY"); 
  addElementToDataset(result, dataset, new DcmCodeString(DoseUnits), "1", "1", "RTDoseModule");
  DcmCodeString DoseType(DCM_DoseType); DoseType.putString("MONTECARLO"); 
  addElementToDataset(result, dataset, new DcmCodeString(DoseType), "1", "1", "RTDoseModule");
  DcmIntegerString InstanceNumber(DCM_InstanceNumber); InstanceNumber.putString("1");
  addElementToDataset(result, dataset, new DcmIntegerString(InstanceNumber), "1", "3", "RTDoseModule");
  DcmCodeString DoseSummationType(DCM_DoseSummationType); DoseSummationType.putString("BEAM");
  addElementToDataset(result, dataset, new DcmCodeString(DoseSummationType), "1", "1", "RTDoseModule");
  G4String offv = "";
  for( size_t ii = 0; ii < nVoxelZ; ii++ ) {
    if( ii != 0 ) offv += "\\";
    offv += GmGenUtils::itoa(ii*widthZ);
  }
  DcmDecimalString GridFrameOffsetVector(DCM_GridFrameOffsetVector); GridFrameOffsetVector.putString(offv);
  addElementToDataset(result, dataset, new DcmDecimalString(GridFrameOffsetVector), "2-n", "1C", "RTDoseModule");

  // --- RTDVHModule (U) ---
  // --- StructureSetModule (C) ---
  // --- ROIContourModule (C) ---
  // --- RTDoseROIModule (C) ---

  /*  
  //= rtdose.write(*fileformat.getDataset());
  // drtdose.cc:  DRTDoseIOD::write(
  //??  updateAttributes(); =   SOPClassUID.putString(UID_RTDoseStorage);
  //  addElementToDataset(result, dataset, new DcmPersonName(PatientName), "1", "2", "PatientModule");
  // addElementToDataset look at dcmrt/libsrc/drttypes.cc
  */

  // --- SOPCommonModule (M) ---  
  DcmUniqueIdentifier SOPClassUID(DCM_SOPClassUID);
  SOPClassUID.putString(UID_RTDoseStorage);
  addElementToDataset(result, dataset, new DcmUniqueIdentifier(SOPClassUID), "1", "1", "SOPCommonModule");
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

OFCondition DCMSqToRT::addElementToDataset(OFCondition &result,
					   DcmItem &dataset,
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
	result = dataset.insert(element, OFTrue /*replaceOld*/);
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

OFBool DCMSqToRT::checkElementValue(DcmElement &element,
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

void DCMSqToRT::PrintHelp()
{
   G4cout << " -fSqdoseIn   Input file in Sqdose format" << G4endl
	  << " -fRTDoseOut  Output file in RTDOSE format" << G4endl
	  << " -patientPosition   PatientPosition (default is HFS)"<< G4endl; 
}
  
