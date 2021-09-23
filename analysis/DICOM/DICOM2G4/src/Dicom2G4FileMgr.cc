#include "Dicom2G4FileMgr.hh"

#include "DICOM/DICOMBase/include/DicomVImage.hh"
#include "DICOM/DICOMBase/include/DicomParameterMgr.hh"
#include "DICOM/DICOMBase/include/DicomMgr.hh"
#include "DICOM/DICOMBase/include/DicomOperCompressXY.hh"
#include "DICOM/DICOMBase/include/DicomOperResize.hh"
#include "DICOM/DICOMBase/include/DicomVerbosity.hh"

#include "DICOM/DICOMReaders/include/DicomVReaderImage.hh"
#include "DICOM/DICOMReaders/include/DicomReaderMgr.hh"
#include "DICOM/DICOMReaders/include/DicomReaderCT.hh"
#include "DICOM/DICOMReaders/include/DicomReaderNM.hh"
#include "DICOM/DICOMReaders/include/DicomReaderRTDose.hh"
#include "DICOM/DICOMReaders/include/DicomReaderRTStruct.hh"

#include "DICOM/DICOM2G4/include/DicomReaderRTPlan.hh"
#include "DICOM/DICOM2G4/include/DicomReaderRTIonPlan.hh"

#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmdata/dcuid.h"

Dicom2G4FileMgr* Dicom2G4FileMgr::theInstance = 0;

//-----------------------------------------------------------------------------
Dicom2G4FileMgr* Dicom2G4FileMgr::GetInstance()
{
  if( !theInstance ) {
    theInstance = new Dicom2G4FileMgr;
  }
  return theInstance;
}

//-----------------------------------------------------------------------------
Dicom2G4FileMgr::Dicom2G4FileMgr()
{
  fCompression = 1.;
  theStructureNCheck = 4;
  
  theDicomMgr = DicomMgr::GetInstance();

}

//-----------------------------------------------------------------------------
void Dicom2G4FileMgr::Convert( G4String fileName )
{
  bFileOutByUser = false;
  std::vector<G4String> wl; 
  //----- Open file
  GmFileIn fin = GmFileIn::GetInstance(fileName, true);
  if( DicomVerb(warningVerb) ) G4cout << "@@@@@@@ Reading FILE: " << fileName << G4endl;

  //----- Read metadata 
  int ii;
  for( ii = 0;; ii++) {
    if( ! fin.GetWordsInLine(wl) ) break;
    if( wl[0] == ":COMPRESSION" ) {
      CheckNColumns(wl,2);
      SetCompression(wl[1]);
    } else if( wl[0] == ":FILE" ) {
      CheckNColumns(wl,2);
      if( DicomVerb(warningVerb) ) G4cout << "@@@@@@@ Reading FILE: " << wl[1]  << G4endl;
      AddFile(wl[1]);
    } else if( wl[0] == ":FILE_OUT" ) {
      CheckNColumns(wl,2);
      theFileOutName = wl[1];
      bFileOutByUser = true;
    } else if( wl[0] == ":MATE_DENS" ) {
      CheckNColumns(wl,3);
      DicomReaderCT::AddMaterialDensity(wl);
    } else if( wl[0] == ":MATE" ) {
      CheckNColumns(wl,3);
      DicomReaderCT::AddMaterial(wl);
    } else if( wl[0] == ":CT2D" ) {
      CheckNColumns(wl,3);
      DicomReaderCT::AddCT2Density(wl);
    } else {      
      G4Exception("Dicom2G4FileMgr::Convert",
		  "",
		  FatalErrorInArgument,
		  G4String("UNKNOWN TAG IN FILE  "+wl[0]).c_str());
    }
    
  }

  //----- Process files
  ProcessFiles();
  
}

//-----------------------------------------------------------------------------
void Dicom2G4FileMgr::CheckNColumns(std::vector<G4String> wl, size_t vsizeTh )
{
  if( wl.size() != vsizeTh ) {
    G4cerr << " Reading line " << G4endl;
    for( size_t ii = 0; ii < wl.size(); ii++){
      G4cerr << wl[ii] << " ";
    }
    G4cerr << G4endl;
    G4Exception("DICOM2G4",
		"",
		FatalErrorInArgument,
		("Wrong number of columns in line " + std::to_string(wl.size()) + " <> " + std::to_string(vsizeTh)).c_str());
  }

}

//-----------------------------------------------------------------------------
void Dicom2G4FileMgr::SetCompression( G4String fComp )
{
  fCompression = GmGenUtils::GetValue(fComp);
}

//-----------------------------------------------------------------------------
void Dicom2G4FileMgr::AddFile( G4String fileName )
{
  DcmFileFormat dfile;
  if( ! (dfile.loadFile(fileName.c_str())).good() ) {
    G4Exception("DicomHandler::ReadFile",
		"",
		FatalErrorInArgument,
		("Error reading file " + fileName).c_str());
  }
  DcmDataset* dset = dfile.getDataset();
  //  G4cout << " Dicom2G4FileMgr dset " << dset << " " << fileName << G4endl; //GDEB
  OFString dModality;
  // https://wiki.cancerimagingarchive.net/display/Public/DICOM+Modality+Abbreviations
  if( !dset->findAndGetOFString(DCM_Modality,dModality).good() ) {
    G4Exception("DicomHandler::ReadData ",
		"",
		FatalException,
		" Have not read Modality");
  }

  OFString sopClassUID;
  if( !dset->findAndGetOFString(DCM_SOPClassUID,sopClassUID).good() ) {
    G4Exception("Dicom2G4FileMgr::ReadData ",
		"",
		FatalException,
		" Have not read SOPClassUID");
  }
  
  if( sopClassUID == UID_CTImageStorage 
    || sopClassUID == UID_RTImageStorage ) {   
    //  if( dModality == "CT" ) {
    DicomReaderCT* df = new DicomReaderCT(dset);
    if( fCompression != 1 ) df->SetCompression( fCompression );
  } else if( sopClassUID == UID_NuclearMedicineImageStorage
	     || sopClassUID == UID_PositronEmissionTomographyImageStorage 
	     || sopClassUID == UID_EnhancedPETImageStorage ) {   
    //  } else if( dModality == "PT" ) {
    //  } else if( dModality == "NM" ) {
    DicomReaderNM* df = new DicomReaderNM(dset);
    if( fCompression != 1 ) df->SetCompression( fCompression );
    } else if( sopClassUID == UID_RTDoseStorage ) {
    //  } else if( dModality == "RTDose" ) {
    DicomReaderRTDose* df = new DicomReaderRTDose(dset);
    if( fCompression != 1 ) df->SetCompression( fCompression );
  } else if( sopClassUID == UID_RTStructureSetStorage ) {
    //  } else if( dModality == "RTStruct" ) {     
    new DicomReaderRTStruct(dset);
  } else if( sopClassUID == UID_RTPlanStorage ) {
 //  } else if( dModality == "RTPlan" ) {
    DicomReaderRTPlan* df = new DicomReaderRTPlan(dset);
    df->SetFileName( fileName );
  } else if( sopClassUID == UID_RTIonPlanStorage ) {
 //  } else if( dModality == "RTIonPlan" ) {
    DicomReaderRTIonPlan* df = new DicomReaderRTIonPlan(dset);
    df->SetFileName( fileName );
  } else {
    G4Exception("DicomHandler::ReadData ",
		"",
		FatalException,
		(G4String("File is not of type CT or NM or RTStruct or RTPlan or RTIonPLAN, but: ") + sopClassUID).c_str());
  }
  
}

//-----------------------------------------------------------------------------
void Dicom2G4FileMgr::ProcessFiles()
{
  theReaderMgr = DicomReaderMgr::GetInstance();
  if( theReaderMgr->GetNofReaders() == 0 ) {
    G4Exception("Dicom2G4FileMgr::ProcessFiles",
		"",
		FatalException,
		"no :FILE provided in metadata file");
  }    

  theReaderMgr->CreateImages();
  
  std::vector<DicomVReaderImage*> CTReaders = theReaderMgr->GetImageReaders(DRM_CT, false);
  theCTReaderAll = 0;
  //  G4cout << " GDEB Dicom2G4FileMgr::ProcessFiles " << CTReaders.size() << G4endl; //GDEB
  if( CTReaders.size() != 0 ) theCTReaderAll = (DicomReaderCT*)CTReaders[0];

  //  G4cout << "GDEB Dicom2G4FileMgr::ProcessFiles  " << CTReaders[0] << " " << theCTReaderAll << G4endl;  //GDEB
  std::vector<DicomVReaderImage*> NMReaders = theReaderMgr->GetImageReaders(DRM_NM, false);
  theNMReaderAll = 0;
  if( NMReaders.size() != 0 ) theNMReaderAll = (DicomReaderNM*)NMReaders[0];

  std::vector<DicomVReaderImage*> RTDoseReaders = theReaderMgr->GetImageReaders(DRM_RTDose, false);
  theRTDoseReaderAll = 0;
  if( RTDoseReaders.size() != 0 ) theRTDoseReaderAll = (DicomReaderRTDose*)RTDoseReaders[0];
  //  G4cout << theRTDoseReaderAll << " Dicom2G4FileMgr DicomReaderRTDose RTDoseReaders " << RTDoseReaders[0] << " IMAGE " << theRTDoseReaderAll->GetImage() << G4endl; //GDEB


  std::vector<DicomVReader*> RTStructReaders = theReaderMgr->GetReaders(DRM_RTStruct, false);
  theRTStructReader = 0;
  if( RTStructReaders.size() != 0 ) theRTStructReader = (DicomReaderRTStruct*)RTStructReaders[0];
  if( RTStructReaders.size() > 1 ) {
    G4Exception("Dicom2G4FileMgr::ProcessFiles",
		"",
		FatalException,
		"more then one RTStruct file");
  }

  std::vector<DicomVReader*> RTPlanReaders = theReaderMgr->GetReaders(DRM_RTPlan,false);
  theRTPlanReader = 0;
  if( RTPlanReaders.size() != 0 ) theRTPlanReader = (DicomReaderRTPlan*)RTPlanReaders[0];

  std::vector<DicomVReader*> RTIonPlanReaders = theReaderMgr->GetReaders(DRM_RTIonPlan,false);
  theRTIonPlanReader = 0;
  if( RTIonPlanReaders.size() != 0 ) theRTIonPlanReader = (DicomReaderRTIonPlan*)RTIonPlanReaders[0];

  DicomMgr::GetInstance()->OperateAll();

  DumpToTextFile();

}

//-----------------------------------------------------------------------------
void Dicom2G4FileMgr::DumpToTextFile()
{
  if( theCTReaderAll != 0 ) {
    if( !bFileOutByUser ) theFileOutName = "testCT.g4dcm";
    if( DicomVerb(warningVerb) ) G4cout << " Dicom2G4FileMgr::Dumping CT To Text File " << theFileOutName << G4endl;
    std::ofstream fout(theFileOutName);
    
#ifndef WIN32
    if( !DicomReaderCT::IsMaterialDensities() ) {
      static std::map<G4double, G4String> materials = DicomReaderCT::GetMaterials();
#else
    if( !theCTReaderAll->IsMaterialDensities()) {
      static std::map<G4double, G4String> materials = theCTReaderAll->GetMaterials();
#endif
      fout << materials.size() << std::endl;
      std::map<G4double,G4String>::const_iterator ite;
      G4int ii = 0;
      for(ite = materials.begin(); ite != materials.end(); ite++, ii++){
	fout << ii << " \"" << (*ite).second << "\"" << std::endl;
      }
      //      if( DicomVerb(warningVerb) ) G4cout << " DUMPED MATE IDS " << materials.size() << G4endl;
    } else {
#ifndef WIN32
      static std::map<G4double, G4String> materialDensities = DicomReaderCT::GetMaterialDensities();
#else
      static std::map<G4double, G4String> materialDensities = theCTReaderAll->GetMaterialDensities();
#endif
      fout << materialDensities.size() << std::endl;
      std::map<G4double,G4String>::const_iterator ite;
      G4int ii = 0;
      for(ite = materialDensities.begin(); ite != materialDensities.end(); ite++, ii++){
	fout << ii << " \"" << (*ite).second << "\"" << std::endl;
      }
      
      //      G4cout << " DUMPED MATE DENS " << materialDensities.size() << G4endl; //GDEB
    }
    
    DicomVImage* imageDicomMateID = theCTReaderAll->GetMateIDImage();
    //    G4cout << "GDEB Dicom2G4FileMgr::DumpToTextFile  " << theCTReaderAll << " : " << imageDicomMateID << G4endl;  //GDEB
    imageDicomMateID->DumpHeaderToTextFile( fout );
    imageDicomMateID->DumpDataToTextFile( fout );
    
    DicomVImage* imageDicomMateDens = theCTReaderAll->GetMateDensImage();
    imageDicomMateDens->DumpDataToTextFile( fout, true );
    
    if( theRTStructReader != 0 ) {
      DicomVImage* imageDicomStructures = theRTStructReader->GetStructImage();
      imageDicomStructures->DumpDataToTextFile( fout );
      std::map<G4int,DicomROI*> rois = theRTStructReader->GetROIs();
      std::map<G4int,DicomROI*>::const_iterator iter; 
      for( iter = rois.begin(); iter != rois.end(); iter++ ) {
	DicomROI* roi = (*iter).second;
	fout << roi->GetNumber() << " \"" << roi->GetName() << "\"" <<G4endl;
      }
    }
    
    fout.close();
  }
    
  if( theNMReaderAll != 0 ) {
    if( !bFileOutByUser ) theFileOutName = "testNM.g4dcm";
    if( DicomVerb(warningVerb) ) G4cout << " Dicom2G4FileMgr::Dumping NM To Text File " << theFileOutName << G4endl;
    std::ofstream fout(theFileOutName);
    DicomVImage* imageDicomNM = theNMReaderAll->GetImage();
    imageDicomNM->DumpHeaderToTextFile( fout );
    imageDicomNM->DumpDataToTextFile( fout );
  }

  if( theRTDoseReaderAll != 0 ) {
    if( !bFileOutByUser ) theFileOutName = "testDose.g4dcm";
    if( DicomVerb(warningVerb) ) G4cout << " Dicom2G4FileMgr::Dumping RTDose To Text File " << theFileOutName << G4endl;
    std::ofstream fout(theFileOutName);
    DicomVImage* imageDicomRTDose = theRTDoseReaderAll->GetImage();
    imageDicomRTDose->DumpHeaderToTextFile( fout );
    imageDicomRTDose->DumpDataToTextFile( fout );

    if( !bFileOutByUser ) {
      theFileOutName = "testDose.sqdose";
    } else {
      theFileOutName += ".sqdose";
    }
    FILE* foutf = fopen((theFileOutName).c_str(),"wb");
    imageDicomRTDose->DumpToSqdoseFile( foutf);

  }

  if( theRTPlanReader != 0 ) {
    theRTPlanReader->SetControlPointMetersets();
    theRTPlanReader->DumpToFile();
  }

  if( theRTIonPlanReader != 0 ) {
    theRTIonPlanReader->SetControlPointMetersets();
    theRTIonPlanReader->DumpToFile();
  }
    
}

