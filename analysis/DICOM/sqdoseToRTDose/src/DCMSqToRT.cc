#include "DCMSqToRT.hh"
#include "DICOM/DICOMBase/include/DicomVerbosity.hh"
#include "DICOM/DICOMBase/include/DicomParameterMgr.hh"
#include "DICOM/DICOMReaders/include/DicomReaderMgr.hh"
#include "DICOM/DICOMReaders/include/DicomUIDInfo.hh"
#include "DICOM/DICOMReaders/include/DicomReaderSqdose.hh"
#include "DICOM/DICOMReaders/include/DicomReaderRTDose.hh"

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
  } else if( argc == 4 ) {    
    theSqDoseInFN = argv[1];
    theRTDoseOutFN = argv[2];
    theRTDoseReferenceFN = argv[3];
    if( theSqDoseInFN[0] == '-' || theRTDoseOutFN[0] == '-' || theRTDoseReferenceFN[0] == '-' ) {
      G4Exception(theExeName,
		  "",
		  FatalErrorInArgument,
		  "THREE ARGUMENTS MUST BE SQDOSE_INPUT_FILE RTDOSE_OUTPUT_FILE RTDOSE_REFERENCE_FILE");
    }
    G4cout << " theSqDoseInFN " << theSqDoseInFN << " theRTDoseOutFN " << theRTDoseOutFN << " thePatientPosition " << thePatientPosition << G4endl;
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
      } else if( argvstr == "-fRTDoseRef" ) {
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
  theSqDoseInFN = theParamMgr->GetStringValue("fSqdoseIn",theSqDoseInFN);
  theRTDoseOutFN = theParamMgr->GetStringValue("fRTDoseOut",theRTDoseOutFN);
  theRTDoseReferenceFN = theParamMgr->GetStringValue("fRTDoseRef",theRTDoseReferenceFN);
  thePatientPosition = theParamMgr->GetStringValue("patientPosition","HFS");
  G4cout << " theSqDoseInFN " << theSqDoseInFN << " theRTDoseOutFN " << theRTDoseOutFN << " theRTDoseReferenceFN " << theRTDoseReferenceFN << " thePatientPosition " << thePatientPosition << G4endl;
}

//---------------------------------------------------------------------------
void DCMSqToRT::CheckArguments()
{
  //------ At least one image file!
  if( theSqDoseInFN == "" ) {
    PrintHelp();
    G4Exception(theExeName,
		"No SqDoseIn input file",
		FatalException,
		"Use -fSqdoseIn in command line");
  }
  if( theRTDoseOutFN == "" ) {
    PrintHelp();
    G4Exception(theExeName,
		"No RTDoseOut output file",
		FatalException,
		"Use -fRTDoseOut in command line");
  }
  // ONE DICOM FILE TO SET UID VALUES
  if( theRTDoseReferenceFN == "" ) {
    PrintHelp();
    G4Exception(theExeName,
		"No RTDose reference file",
		FatalException,
		"Use -fRTDoseRef in command line");
  }

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
  /*t FOR CASES WHEN THERE IS NO RTDOSE file
   G4int ndcm = theParamMgr->GetNofImageFiles("fCT");
   ndcm += theParamMgr->GetNofImageFiles("fNM");
   ndcm += theParamMgr->GetNofImageFiles("fRTDOSE");
   if( ndcm > 1 ) {
    G4Exception("DCMSqToRT::CheckArguments","",FatalException,"THERE CAN ONLY BE ONE IMAGE OF TYPES CT, NM AND RTDOSE");
    }*/
 if( theParamMgr->GetNofImageFiles("fCT") > 1 ) {
    G4Exception("DCMSqToRT::CheckArguments","",FatalException,"THERE CANNOT BE ANY IMAGE FILE OF TYPE CT");
  }
  if( theParamMgr->GetNofImageFiles("fNM") > 1 ) {
    G4Exception("DCMSqToRT::CheckArguments","",FatalException,"THERE CANNOT BE ANY IMAGE FILE OF TYPE NM");
  }
  
}

//---------------------------------------------------------------------------
void DCMSqToRT::ConvertImages()
{
  //-----  Get DicomUIDInfo from one of the DICOM files
  //--- READ DICOM FILES FOR UID
  //  theReaderMgr->ProcessData(); // do it by hand, as ReaderMgr will look for -fSqdose, not -fSqdoseIn
  DicomReaderSqdose* readerSqIn = new DicomReaderSqdose(theSqDoseInFN);
  //new DicomReaderRTDose(theRTDoseOutFN);
  DicomReaderRTDose* readerRTDRef = new DicomReaderRTDose(theRTDoseReferenceFN);
  theReaderMgr->CreateImages();
  
  theDicomUIDInfo = readerRTDRef->GetUIDInfo();
  bUIDfromRT = true;

  //--------- READ Sqdose
  GmSqdose* sqdose = readerSqIn->GetSqdose(); 
  /*  if( DicomVerb(infoVerb) ) G4cout << "@@@@@@@ READING SQDOSE FILE: " << theSqDoseInFN << G4endl;
  sqdose = new GmSqdose();
  sqdose->Read(theSqDoseInFN);
  //    sqdose->CalculateErrors();
  */
  
  DcmDataset* dataset = new DcmDataset();
  //DRTDoseIOD rtdose;
  fillRTDoseFromSqdose( sqdose, dataset );

  //  G4cout << " @@@ DATASET " << G4endl; //GDEB
 // dataset->print(G4cout); //GDEB
  //  DcmOutputStream rtout(theRTDoseOutFN+"ds");
  //dataset->write(rtout);
  // CAN FILL DATASET DIRECTLY, WITHOUT DRTDoseIOD ???
  DcmFileFormat fileFormat( dataset );
  if( DicomVerb(infoVerb) )   G4cout << "@@@@@@@ SAVING RTDOSE FILE " << theRTDoseOutFN << G4endl;

  OFCondition status = fileFormat.saveFile(OFFilename(theRTDoseOutFN),
    EXS_LittleEndianExplicit,
    EET_ExplicitLength,   // ← this is what controls undefined vs explicit length
    EGL_recalcGL
);
  //  OFCondition status = fileFormat.saveFile(OFFilename(theRTDoseOutFN),EXS_LittleEndianImplicit);
  if (status.bad()) {
    std::cerr << "Error: cannot save DICOM file (" << status.text() << ")" << std::endl;
  }
}

//---------------------------------------------------------------------------
void DCMSqToRT::fillRTDoseFromSqdose( GmSqdose* sqdose, DcmDataset* dataset )
{
  Uint16 theNBits = 16;
  GmSqdoseHeader* sqdoseHeader = sqdose->GetHeader();
  size_t nVoxelX = sqdoseHeader->GetNoVoxelsX();
  size_t nVoxelY = sqdoseHeader->GetNoVoxelsY();
  size_t nVoxelZ = sqdoseHeader->GetNoVoxelsZ();
  unsigned long nVoxels = nVoxelX*nVoxelY*nVoxelZ;
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
  G4String myUID = theDicomUIDInfo->GetInfo(DCM_MediaStorageSOPInstanceUID);

  /*  DcmUniqueIdentifier TransferSyntaxUID(DCM_TransferSyntaxUID);
  if( GmGenUtils::IsLittleEndian() ) {
    TransferSyntaxUID.putString("LittleEndianExplicit");
  } else {
    TransferSyntaxUID.putString("BigEndianExplicit");
  }
  addElementToDataset(result, dataset, new DcmUniqueIdentifier(TransferSyntaxUID), "1", "1", "MetaInformationHeader");
  */

  DcmUniqueIdentifier MediaStorageSOPClassUID(DCM_MediaStorageSOPClassUID); MediaStorageSOPClassUID.putString(UID_RTDoseStorage);
  addElementToDataset(result, dataset, new DcmUniqueIdentifier(MediaStorageSOPClassUID), "1", "1", "SOPCommonModule");
  SetFromReference( dataset, DCM_MediaStorageSOPInstanceUID, "1", "1", "MetaInformationHeader");
  
  SetFromReference( dataset,DCM_ImplementationClassUID, "1", "1", "MetaInformationHeader");
  
  SetFromReference( dataset,DCM_ImplementationVersionName, "1", "1", "MetaInformationHeader","ShortString");
  SetFromReference( dataset,DCM_PrivateInformationCreatorUID, "1", "1", "MetaInformationHeader");
  G4cout << " @@@ DATASET PrivateInformationCreatorUID " << G4endl; //GDEB
  dataset->print(G4cout); //GDEB  
  SetFromReference( dataset,DCM_PrivateInformation, "1", "1", "MetaInformationHeader","OtherByteOtherWord");
  G4cout << " @@@ DATASET PrivateInformation  " << G4endl; //GDEB
  dataset->print(G4cout); //GDEB  

  SetFromReference( dataset,DCM_SpecificCharacterSet, "1", "1", "GeneralStudyModule","CodeString");
  DcmDate InstanceCreationDate(DCM_InstanceCreationDate); InstanceCreationDate.putString(date);
  addElementToDataset(result, dataset, new DcmDate(InstanceCreationDate), "1", "3", "SOPCommonModule");
  DcmTime InstanceCreationTime(DCM_InstanceCreationTime); InstanceCreationTime.putString(time);
  addElementToDataset(result, dataset, new DcmTime(InstanceCreationTime), "1", "3", "SOPCommonModule");
  DcmUniqueIdentifier SOPClassUID(DCM_SOPClassUID);
  SOPClassUID.putString(UID_RTDoseStorage);
  addElementToDataset(result, dataset, new DcmUniqueIdentifier(SOPClassUID), "1", "1", "SOPCommonModule");
  SetFromReference( dataset,DCM_SOPInstanceUID, "1", "1", "SOPCommonModule");
  DcmDate StudyDate(DCM_StudyDate); StudyDate.putString(date);
  addElementToDataset(result, dataset, new DcmDate(StudyDate), "1", "2", "GeneralStudyModule");
  DcmDate SeriesDate(DCM_SeriesDate); SeriesDate.putString(date);
  addElementToDataset(result, dataset, new DcmDate(SeriesDate), "1", "2", "GeneralStudyModule");
  DcmDate ContentDate(DCM_ContentDate); ContentDate.putString(date);
  addElementToDataset(result, dataset, new DcmDate(ContentDate), "1", "2", "GeneralStudyModule");
  DcmTime StudyTime(DCM_StudyTime); StudyTime.putString(time);
  addElementToDataset(result, dataset, new DcmTime(StudyTime), "1", "2", "GeneralStudyModule");
  DcmTime SeriesTime(DCM_SeriesTime); SeriesTime.putString(time);
  addElementToDataset(result, dataset, new DcmTime(SeriesTime), "1", "2", "GeneralStudyModule");
  DcmTime ContentTime(DCM_ContentTime); ContentTime.putString(time);
  addElementToDataset(result, dataset, new DcmTime(ContentTime), "1", "2", "GeneralStudyModule");
  addElementToDataset(result, dataset, new DcmShortString(DCM_AccessionNumber), "1", "2", "GeneralStudyModule"); //(no value available)            
  DcmCodeString Modality(DCM_Modality); Modality.putString("RTDOSE");
  addElementToDataset(result, dataset, new DcmCodeString(Modality), "1", "1", "RTSeriesModule");

  // --- GeneralEquipmentModule (M) ---
  //  DcmLongString Manufacturer(DCM_Manufacturer); Manufacturer.putString("GAMOS");
  // addElementToDataset(result, dataset, new DcmLongString(Manufacturer), "1", "3", "GeneralEquipmentModule");
  SetFromReference( dataset,DCM_Manufacturer, "1", "1", "GeneralStudyModule","LongString");
  //  DcmLongString InstitutionName(DCM_InstitutionName); InstitutionName.putString("CIEMAT");
  //  addElementToDataset(result, dataset, new DcmLongString(InstitutionName), "1", "3", "GeneralEquipmentModule");
  SetFromReference( dataset,DCM_InstitutionName, "1", "1", "GeneralStudyModule","LongString");
  
  addElementToDataset(result, dataset, new DcmPersonName(DCM_ReferringPhysicianName), "1", "2", "GeneralStudyModule");
  SetFromReference( dataset,DCM_SeriesDescription, "1", "1", "GeneralStudyModule");
  addElementToDataset(result, dataset, new DcmPersonName(DCM_OperatorsName), "1", "2", "GeneralStudyModule");
  //  DcmLongString ManufacturerModelName(DCM_ManufacturerModelName); ManufacturerModelName.putString("GAMOS");
  // addElementToDataset(result, dataset, new DcmLongString(ManufacturerModelName), "1", "3", "GeneralEquipmentModule");
  SetFromReference( dataset,DCM_ManufacturerModelName, "1", "1", "GeneralStudyModule","LongString");
  
  // --- PatientModule (M) ---
  SetFromReference( dataset,DCM_PatientName, "1", "1", "PatientModule","PersonName");
  SetFromReference( dataset,DCM_PatientID, "1", "1", "PatientModule","LongString");
  SetFromReference( dataset,DCM_PatientBirthDate, "1", "1", "PatientModule","Date");
  SetFromReference( dataset,DCM_PatientSex, "1", "1", "PatientModule","CodeString");
  
  DcmDecimalString SliceThickness(DCM_SliceThickness); SliceThickness.putString(GmGenUtils::ftoa(widthZ));
  addElementToDataset(result, dataset, new DcmDecimalString(SliceThickness), "1", "2", "ImagePlaneModule");
  
  //  DcmLongString SoftwareVersions(DCM_SoftwareVersions); SoftwareVersions.putString(getenv("GAMOS_VER"));
  // addElementToDataset(result, dataset, new DcmLongString(SoftwareVersions), "1-n", "3", "GeneralEquipmentModule");
  SetFromReference( dataset,DCM_SoftwareVersions, "1", "1", "GeneralStudyModule","LongString");

  // --- GeneralStudyModule (M) ---
  SetFromReference( dataset,DCM_StudyInstanceUID, "1", "1", "GeneralStudyModule");
  SetFromReference( dataset,DCM_SeriesInstanceUID, "1", "1", "GeneralStudyModule");
  addElementToDataset(result, dataset, new DcmShortString(DCM_StudyID), "1", "2", "GeneralStudyModule"); //(no value available)            
  SetFromReference( dataset,DCM_SeriesNumber, "1", "1", "GeneralStudyModule","IntegerString");
  DcmIntegerString InstanceNumber(DCM_InstanceNumber); InstanceNumber.putString("3");
  addElementToDataset(result, dataset, new DcmIntegerString(InstanceNumber), "1", "1", "GeneralEquipmentModule");
  G4cout << " @@@ DATASET InstanceNumber " << G4endl; //GDEB
  dataset->print(G4cout); //GDEB  

  // imagePositionPatient: hadd 1/2 voxel size 
  double PixelWidth = widthX;
  double PixelHeight = widthY;
  G4double imagePosPatX = voxelLimitsX[0] + 0.5*PixelWidth;
  G4double imagePosPatY = voxelLimitsY[0] + 0.5*PixelHeight;
  G4double imagePosPatZ = voxelLimitsZ[0] + 0.5*widthZ;
  G4cout << " imagePosPatZ " << imagePosPatZ << " " << voxelLimitsZ[0] << "+ *widthZ" << widthZ<< G4endl;//GDEB
  G4String imagePos = GmGenUtils::ftoa(imagePosPatX) + "\\" +
    GmGenUtils::ftoa(imagePosPatY) + "\\" +
    GmGenUtils::ftoa(imagePosPatZ);  
  DcmDecimalString ImagePositionPatient(DCM_ImagePositionPatient); ImagePositionPatient.putString(imagePos);
  addElementToDataset(result, dataset, new DcmDecimalString(ImagePositionPatient), "3", "1", "ImagePlaneModule");
  G4String imageOrient = "1\\0\\0\\0\\1\\0";
  DcmDecimalString ImageOrientationPatient(DCM_ImageOrientationPatient); ImageOrientationPatient.putString(imageOrient);
  addElementToDataset(result, dataset, new DcmDecimalString(ImageOrientationPatient), "6", "1", "ImagePlaneModule");
    G4cout << " @@@ DATASET ImageOrientationPatient " << G4endl; //GDEB
  dataset->print(G4cout); //GDEB  

  // --- FrameOfReferenceModule (M) ---
  SetFromReference( dataset,DCM_FrameOfReferenceUID, "1", "1", "FrameOfReferenceModule");
  addElementToDataset(result, dataset, new DcmLongString(DCM_PositionReferenceIndicator), "1", "2", "FrameOfReferenceModule");
  
  DcmUnsignedShort SamplesPerPixel(DCM_SamplesPerPixel); SamplesPerPixel.putUint16(1);
  addElementToDataset(result, dataset, new DcmUnsignedShort(SamplesPerPixel), "1", "1C", "RTDoseModule");
  DcmCodeString PhotometricInterpretation(DCM_PhotometricInterpretation); PhotometricInterpretation.putString("MONOCHROME2"); 
  addElementToDataset(result, dataset, new DcmCodeString(PhotometricInterpretation), "1", "1C", "RTDoseModule");
  // --- MultiFrameModule (C) ---
  DcmIntegerString NumberOfFrames(DCM_NumberOfFrames); NumberOfFrames.putString(GmGenUtils::itoa(nVoxelZ));
  addElementToDataset(result, dataset, new DcmIntegerString(NumberOfFrames), "1", "1", "MultiFrameModule");
  SetFromReference( dataset,DCM_FrameIncrementPointer, "1", "1", "PatientModule","AttributeTag");
		      
  // --- ImagePixelModule (C) ---
  DcmUnsignedShort Rows(DCM_Rows); Rows.putUint16(nVoxelY);
  addElementToDataset(result, dataset, new DcmUnsignedShort(Rows), "1", "1", "ImagePixelModule");
  DcmUnsignedShort Columns(DCM_Columns); Columns.putUint16(nVoxelX);
  addElementToDataset(result, dataset, new DcmUnsignedShort(Columns), "1", "1", "ImagePixelModule");
  char buffer[32];
  OFStandard::ftoa(buffer, 15, PixelWidth, OFStandard::ftoa_format_f);
  strcat(buffer, "\\");
  OFStandard::ftoa(strchr(buffer, 0), 15, PixelHeight, OFStandard::ftoa_format_f);
  DcmDecimalString PixelSpacing(DCM_PixelSpacing); PixelSpacing.putString(buffer);
  addElementToDataset(result, dataset, new DcmDecimalString(PixelSpacing), "2", "1", "ImagePlaneModule");
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
  DcmCodeString DoseType(DCM_DoseType); DoseType.putString("PHYSICAL"); 
  addElementToDataset(result, dataset, new DcmCodeString(DoseType), "1", "1", "RTDoseModule");
  addElementToDataset(result, dataset, new DcmLongString(DCM_DoseComment), "1", "2", "GeneralStudyModule"); //(no value available)            

  SetFromReference( dataset,DCM_DoseSummationType, "1", "1", "","CodeString");
  G4String offv = "";
  for( size_t ii = 0; ii < nVoxelZ; ii++ ) {
    if( ii != 0 ) offv += "\\";
    offv += GmGenUtils::itoa(ii*widthZ);
  }
  DcmDecimalString GridFrameOffsetVector(DCM_GridFrameOffsetVector); GridFrameOffsetVector.putString(offv);
  addElementToDataset(result, dataset, new DcmDecimalString(GridFrameOffsetVector), "2-n", "1C", "RTDoseModule");

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

  DcmCodeString TissueHeterogeneityCorrection(DCM_TissueHeterogeneityCorrection); TissueHeterogeneityCorrection.putString("IMAGE\\ROI_OVERRIDE");
  addElementToDataset(result, dataset, new DcmCodeString(TissueHeterogeneityCorrection), "1", "3", "GeneralEquipmentModule");

  /*  DcmCodeString PatientPosition(DCM_PatientPosition); PatientPosition.putString(thePatientPosition);
      addElementToDataset(result, dataset, new DcmCodeString(PatientPosition), "1", "1", "GeneralSeriesModule"); //extra*/

  //  if (result.good()) result = ReferencedPatientPhotoSequence.write(dataset, "1-n" ,"3", "PatientModule");
  // addElementToDataset(result, dataset, new DcmCodeString(QualityControlSubject), "1", "3", "PatientModule");
  //  if (result.good()) result = ReferencedPatientSequence.write(dataset, "1-n" ,"3", "PatientModule");

  
  // --- PatientStudyModule (U) ---
  // --- ClinicalTrialSubjectModule (U) ---
  
  // --- RTSeriesModule (M) ---

  // --- GeneralSeriesModule (M) ---

  // --- ClinicalTrialSeriesModule (U) ---

  // --- GeneralImageModule (C) ---
  // --- ImagePlaneModule (C) ---

  Uint16* pd = new Uint16[nVoxels];
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

  // (300C,0002) ReferencedRTPlanSequence
  DcmSequenceOfItems* referencedRTPlanSeq = new DcmSequenceOfItems(DCM_ReferencedRTPlanSequence);
  //-  referencedRTPlanSeq->setEncodeWithExplicitLength(OFTrue);
  DcmItem* item = new DcmItem();
  //  item->setEncodeWithExplicitLength(OFTrue);
  //-  DcmUniqueIdentifier ReferencedSOPClassUID(DCM_ReferencedSOPClassUID); 
  //- DcmDecimalString DoseGridScaling(DCM_DoseGridScaling); DoseGridScaling.putString(GmGenUtils::ftoa(1./factor));
  //  item->putAndInsertString(DCM_ReferencedSOPClassUID, UID_RTIonPlanStorage);
  item->putAndInsertString(DCM_ReferencedSOPClassUID, theDicomUIDInfo->GetInfo(DCM_ReferencedSOPClassUID));
  item->putAndInsertString(DCM_ReferencedSOPInstanceUID, theDicomUIDInfo->GetInfo(DCM_ReferencedSOPInstanceUID) ); 

  referencedRTPlanSeq->insert(item);
  dataset->insert(referencedRTPlanSeq, OFTrue);
  
  DcmPixelData PixelData(DCM_PixelData); PixelData.putUint16Array (pd,nVoxels);
  //utUint16Array (c(const Uint16 *wordValue, const unsigned long length);
  addElementToDataset(result, dataset, new DcmPixelData(PixelData), "1", "1C", "ImagePixelModule");

  /*  DcmUnsignedShort SmallestImagePixelValue(DCM_SmallestImagePixelValue); SmallestImagePixelValue.putUint16(0);
  addElementToDataset(result, dataset, new DcmUnsignedShort(SmallestImagePixelValue), "1", "3", "ImagePixelModule");
  DcmUnsignedShort LargestImagePixelValue(DCM_LargestImagePixelValue); LargestImagePixelValue.putUint16(pow(2,theNBits)-1);
  addElementToDataset(result, dataset, new DcmUnsignedShort(LargestImagePixelValue), "1", "3", "ImagePixelModule");
  */
  
  //  addElementToDataset(result, dataset, new DcmAttributeTag(FrameIncrementPointer), "1-n", "1", "MultiFrameModule");

  // --- RTDoseModule (M) ---


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

  // --- CommonInstanceReferenceModule (U) ---
  // --- FrameExtractionModule (C) ---
  
}

//------------------------------------------------------------------------
OFCondition DCMSqToRT::addElementToDataset(OFCondition &result,
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

//------------------------------------------------------------------------
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

//------------------------------------------------------------------------
OFCondition DCMSqToRT::SetFromReference( DcmItem *dataset, const DcmTag &tag, const OFString &vm, const OFString &type, const char *moduleName, G4String elemType)
{
  OFCondition result;
  if( elemType == "UniqId" ) {
    DcmUniqueIdentifier dcmEl(tag);
    dcmEl.putString( theDicomUIDInfo->GetInfo(tag) );
    result = addElementToDataset(result, dataset, new DcmUniqueIdentifier(dcmEl), vm, type, moduleName);
  } else if( elemType == "PersonName" ) {
    DcmPersonName dcmEl(tag);
    dcmEl.putString( theDicomUIDInfo->GetInfo(tag) );
    result = addElementToDataset(result, dataset, new DcmPersonName(dcmEl), vm, type, moduleName);
  } else if( elemType == "LongString" ) {
    DcmLongString  dcmEl(tag);
    dcmEl.putString( theDicomUIDInfo->GetInfo(tag) );
    result = addElementToDataset(result, dataset, new DcmLongString(dcmEl), vm, type, moduleName);
  } else if( elemType == "Date" ) {
    DcmDate  dcmEl(tag);
    dcmEl.putString( theDicomUIDInfo->GetInfo(tag) );
    result = addElementToDataset(result, dataset, new DcmDate(dcmEl), vm, type, moduleName);
  } else if( elemType == "CodeString" ) {
    DcmCodeString  dcmEl(tag);
    dcmEl.putString( theDicomUIDInfo->GetInfo(tag) );
    result = addElementToDataset(result, dataset, new DcmCodeString(dcmEl), vm, type, moduleName);
  } else if( elemType == "Time" ) {
    DcmTime  dcmEl(tag);
    dcmEl.putString( theDicomUIDInfo->GetInfo(tag) );
    result = addElementToDataset(result, dataset, new DcmTime(dcmEl), vm, type, moduleName);
  } else if( elemType == "ShortString" ) {
    DcmShortString  dcmEl(tag);
    dcmEl.putString( theDicomUIDInfo->GetInfo(tag) );
    result = addElementToDataset(result, dataset, new DcmShortString(dcmEl), vm, type, moduleName);
  } else if( elemType == "IntegerString" ) {
    DcmIntegerString  dcmEl(tag);
    dcmEl.putString( theDicomUIDInfo->GetInfo(tag) );
    result = addElementToDataset(result, dataset, new DcmIntegerString(dcmEl), vm, type, moduleName);
  } else if( elemType == "DecimalString" ) {
    DcmDecimalString  dcmEl(tag);
    dcmEl.putString( theDicomUIDInfo->GetInfo(tag) );
    result = addElementToDataset(result, dataset, new DcmDecimalString(dcmEl), vm, type, moduleName);
  } else if( elemType == "UnsignedShort" ) {
    DcmUnsignedShort dcmEl(tag);
    dcmEl.putString( theDicomUIDInfo->GetInfo(tag) );
    result = addElementToDataset(result, dataset, new DcmUnsignedShort(dcmEl), vm, type, moduleName);
  } else if( elemType == "AttributeTag" ) {
    DcmAttributeTag dcmEl(tag);
    dcmEl.putString( theDicomUIDInfo->GetInfo(tag) );
    result = addElementToDataset(result, dataset, new DcmAttributeTag(dcmEl), vm, type, moduleName);
  } else if( elemType == "OtherByteOtherWord" ) {
    DcmOtherByteOtherWord dcmEl(tag);
    dcmEl.putString( theDicomUIDInfo->GetInfo(tag) );
    result = addElementToDataset(result, dataset, new DcmOtherByteOtherWord(dcmEl), vm, type, moduleName);
  } else {
    G4Exception("DCMSqToRT::SetFromReference",
		"",
		FatalErrorInArgument,
		("elemType not found: "+elemType).c_str());
    result = RT_EC_InvalidValue;
  }

  return result;
}

//------------------------------------------------------------------------
void DCMSqToRT::PrintHelp()
{
   G4cout << " -fSqdoseIn   Input file in Sqdose format" << G4endl
	  << " -fRTDoseOut  Output file in RTDOSE format" << G4endl
	  << " -patientPosition   PatientPosition (default is HFS)"<< G4endl; 
}
  
/*
VR to DCMTK class mapping (common ones) VRDCMTK Class
UI DcmUniqueIdentifier
LO DcmLongString
SH DcmShortString
ST DcmShortText
LT DcmLongText
PN DcmPersonName
DA DcmDate
TM DcmTime
DT DcmDateTime
DS DcmDecimalString
IS DcmIntegerString
US DcmUnsignedShort
SS DcmSignedShort
UL DcmUnsignedLong
SL DcmSignedLong
FL DcmFloatingPointSingle
FD DcmFloatingPointDouble
CS DcmCodeString
AS DcmAgeString
SQ DcmSequenceOfItems
OB DcmOtherByteOtherWord
OW DcmOtherByteOtherWord
UN DcmUnknown
*/
