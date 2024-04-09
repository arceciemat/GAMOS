#include "DCMChangeStructure.hh"

#include "DICOM/DICOMBase/include/DicomVImage.hh"
#include "DICOM/DICOMBase/include/DicomMgr.hh"
#include "DICOM/DICOMBase/include/DicomParameterMgr.hh"
#include "DICOM/DICOMBase/include/DicomVerbosity.hh"
#include "DICOM/DICOMReaders/include/DicomReaderMgr.hh"
#include "DICOM/DICOMReaders/include/DicomReaderG4dcmCT.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "CLHEP/Random/RandFlat.h"

#include <iostream>
#include <map>

//---------------------------------------------------------------------------
DCMChangeStructure::DCMChangeStructure()
{
  Initialise();
  theParamMgr = (DicomParameterMgr*)(DicomParameterMgr::GetInstance());
  theReaderMgr = DicomReaderMgr::GetInstance();
  theDicomMgr = DicomMgr::GetInstance();
  bImageLimits = false;
  theImageMinX = -DBL_MAX;
  theImageMaxX = DBL_MAX;
  theImageMinY = -DBL_MAX;
  theImageMaxY = DBL_MAX;
  theImageMinZ = -DBL_MAX;
  theImageMaxZ = DBL_MAX;
}

//---------------------------------------------------------------------------
void DCMChangeStructure::Initialise()
{
  theExeName = "changeStructure";
  theAnalyseImage = 0;
  theMateDensImage = 0;
  theStructImage = 0;

  bChangeMaterials = false;
  bChangeDensities = false;
  theStructDataListFN = "";
  theStructureName = "";
  theMaterialName = "";
  theMaterialID = -1;
  theDensity = -1.;
  bStOnly = 0;
  bGivenStructData = false;
  bImageG4dcmCT = false;
  theOutFN = "";
}
 
//---------------------------------------------------------------------------
void DCMChangeStructure::ProcessArguments(int argc,char** argv)
{
  if( argc == 6 ) {
    theParamMgr->AddParam(G4String("fG4dcmCT ")+argv[1],PTVstring);
    StructData stData;
    stData.stName = argv[2];
    stData.mateName = argv[3];
    stData.density = GmGenUtils::GetValue(argv[4]);
    stData.bOnly = 0;
    theStructData.push_back(stData);
    theOutFN = argv[5];
    if( stData.mateName != "" || stData.mateName != "NONE" ) bChangeMaterials = true;
  } else {
    if(argc%2 != 1) {
      G4Exception(theExeName.c_str(),
		  "Error",
		  FatalErrorInArgument,
		  "WRONG NUMBER OF ARGUMENTS: THEY MUST BE -XX1 VAL_XX1 -XX2 VAL_XX2 ... ");
    }
    for( G4int ii = 1; ii < argc; ii++ ){
      G4String argvstr = argv[ii];
      if( argvstr == "-struct" ) {
	theStructureName = argv[ii+1];
	bGivenStructData = true; 
	ii++;
      } else if( argvstr == "-mate" ) {
	theMaterialName = argv[ii+1];
	bGivenStructData = true;
	if( theMaterialName != "" || theMaterialName != "NONE" ) bChangeMaterials = true;
	ii++;
      } else if( argvstr == "-density" ) {
	theDensity = GmGenUtils::GetValue(argv[ii+1]);
	bChangeDensities = true;
	bGivenStructData = true;
	ii++;
      } else if( argvstr == "-bStructOnly" ) {
	bStOnly = GmGenUtils::GetBool(argv[ii+1]);
	bGivenStructData = true;
	ii++;
      } else if( argvstr == "-stMateDens" ) {
	if( argc < ii+5 ) {
	  G4Exception("changeStructure",
		"Error",
		FatalErrorInArgument,
		"WRONG NUMBER OF ARGUMENTS, THERE MUST BE AT LEAST 4 AFTER -stMateDens");
	}
	StructData stData;
	stData.stName = argv[ii+1];
	stData.mateName = argv[ii+2];
	if( stData.mateName != "" || stData.mateName != "NONE" ) bChangeMaterials = true;
	stData.density = GmGenUtils::GetValue(argv[ii+3]);
	if( stData.density != -1 ) bChangeDensities = true;
	if( stData.mateName == "" && stData.density < 0 ) {
	  G4Exception("changeStructure",
		"Error",
		FatalErrorInArgument,
		"MATERIAL_NAME CANNOT BE EMTPY AND AT THE SAME TIME DENSITY < 0  AFTER -stMateDens");
	}
	stData.bOnly = GmGenUtils::GetBool(argv[ii+4]);
	theStructData.push_back(stData);
	ii+=4;
      } else if( argvstr == "-fChangeList" ) {
	theStructDataListFN = argv[ii+1];
	ii++;
      } else if( argvstr == "-minX" ) {
	theImageMinX = GmGenUtils::GetValue(argv[ii+1]);
	bImageLimits = true;
	ii++;
      } else if( argvstr == "-maxX" ) {
	theImageMaxX = GmGenUtils::GetValue(argv[ii+1]);
	bImageLimits = true;
	ii++;
      } else if( argvstr == "-minY" ) {
	theImageMinY = GmGenUtils::GetValue(argv[ii+1]);
	bImageLimits = true;
	ii++;
      } else if( argvstr == "-maxY" ) {
	theImageMaxY = GmGenUtils::GetValue(argv[ii+1]);
	bImageLimits = true;
	ii++;
      } else if( argvstr == "-minZ" ) {
	theImageMinZ = GmGenUtils::GetValue(argv[ii+1]);
	bImageLimits = true;
	ii++;
      } else if( argvstr == "-maxZ" ) {
	theImageMaxZ = GmGenUtils::GetValue(argv[ii+1]);
	bImageLimits = true;	  
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

}

//---------------------------------------------------------------------------
void DCMChangeStructure::CheckArguments()
{
  if( bImageLimits ) { // for change by Struct it will be che3cked of each st
    if ( !bChangeMaterials && !bChangeDensities ) {
      G4Exception(theExeName,
		  "",
		  FatalException,
		  "Not material nor material density to be changed");
    }
  }
						    
  //    -fDCMIn testCT.c1.g4dcm -fOut testCT.c1_couch1.g4dcm -st Couch_VersaEx -mate G4_C -density 0.65
  //--- THERE MUST BE AN OUTPUT FILE  
  theOutFN = theParamMgr->GetStringValue("fOut",theOutFN);
  if( theOutFN == "" ) {
    G4Exception(theExeName,
		"",
		FatalException,
		"No output file, please use the parameter -fOut");
  }

  size_t nfct = theParamMgr->GetNofImageFiles("fG4dcmCT");
  size_t nother = theParamMgr->GetNofImageFiles("ANY")-nfct;
  //--- THERE MUST BE AN INPUT IMAGE FILE, OR TWO IF SECOND G4dcmCT IS ONLY USED FOR MATERIAL
  if( nfct != 1 ) {
    G4Exception(theExeName, 
		"",
		FatalException,
		("Number of input files of type DIM_G4dcmCT is " + GmGenUtils::itoa(nfct) + ", while it should be 1").c_str());  // Only G4dcmCT containing material information
  }
  if( nother != 0 ) { //- if there is one image file not G4dcmCT, it will be used as image to be changed
    G4Exception(theExeName,
		"",
		FatalException,
		("Number of input files of type different than DIM_G4dcmCT is " + GmGenUtils::itoa(nother) + ", while it should be 1").c_str()); 
  }
}
  
//---------------------------------------------------------------------------
void DCMChangeStructure::ReadFilesAndGetImages()
{
  //--- READ AND BUILD IMAGES
  theReaderMgr->ProcessData();
  std::map<G4int,G4String> theStNames = theReaderMgr->GetStructIDsNames();
  
  //--- GET STRUCT IMAGE (PRIORITY TO DIM_RTStruct)
  std::vector<DicomVImage*> images = theDicomMgr->GetImages(DIM_RTStruct, false);
  if( images.size() != 0 ) {
    theStructImage = dynamic_cast<DicomVImageStr*>(images[0]);
  }
  if( !theStructImage ) {
    images = theDicomMgr->GetImages(DIM_G4dcmCT_StructID);
    if( images.size() != 0 ) {
      theStructImage = dynamic_cast<DicomVImageStr*>(images[0]);
    }
  }
  /*  if( !theStructImage ) {  // already checked that ndcmct = 1 if material is going to be changed
    G4Exception(theExeName,
		"Error",
		FatalException,
		"NO STRUCTURE IMAGE FOUND");
		} */

  //--- GET IMAGE TO ANALYSE. IF OTHER IMAGE THAN G4dcmCT, TAKE IT AND USE G4dcmCT ONLY FOR STRUCT
  bImageG4dcmCT = false;
  images = theDicomMgr->GetImages(DIM_NM,false);
  if( images.size() != 0 ) {
    theAnalyseImage = images[0];
  }
  images = theDicomMgr->GetImages(DIM_RTDose,false);
  if( images.size() != 0 ) {
    theAnalyseImage = images[0];
  }
  images = theDicomMgr->GetImages(DIM_G4dcmNM,false);
  if( images.size() != 0 ) {
    theAnalyseImage = images[0];
  }
  images = theDicomMgr->GetImages(DIM_G4dcmDose,false);
  if( images.size() != 0 ) {
    theAnalyseImage = images[0];
  }
  if( !theAnalyseImage ) {
    images = theDicomMgr->GetImages(DIM_G4dcmCT_MateID);
    if( images.size() != 0 ) {
      theAnalyseImage = images[0];
      bImageG4dcmCT = true;
    }
  }
  if( bChangeMaterials ) {
    images = theDicomMgr->GetImages(DIM_G4dcmCT_MateDens);
    theMateDensImage = images[0];
    //    G4cout << "theMateDensImage " << theMateDensImage << G4endl; //GDEB
  }    

  //  G4cout << "theAnalyseImage " << theAnalyseImage << " " <<bImageG4dcmCT << G4endl; //GDEB
}

//---------------------------------------------------------------------------
void DCMChangeStructure::BuildStructData()
{
  //----- READ StructData FROM FILE
  if( theStructDataListFN != "" ) {
    GmFileIn fin = GmFileIn::GetInstance(theStructDataListFN,true);
    std::vector<G4String> wl;
    for(;;){
      if( !fin.GetWordsInLine(wl) ) break;
      if( wl.size() != 4 ) {
	G4cerr << "LINE READ: ";
	for( size_t ii = 0; ii < wl.size(); ii++ ) {
	  G4cerr << wl[ii] << " ";
	}
	G4cerr << G4endl;
	G4Exception("changeStructure",
		    "",
		    FatalException,
		    "Line should have four words: STRUCTURE_NAME MATERIAL_NAME DENSITY bONLY_IN_THIS_STRUCTURE(0/1)");
      }
      StructData stData;
      stData.stName = wl[0];
      stData.mateName = wl[1];
      if( stData.mateName != "" || stData.mateName != "NONE" ) bChangeMaterials = true;
      stData.density = GmGenUtils::GetValue(wl[2]);
      if( stData.density != -1 ) bChangeDensities = true;
      stData.bOnly = GmGenUtils::GetBool(wl[3]);
      theStructData.push_back(stData);
    }
  }
  
  //----- FILL ONE StructData WITH INFO GIVEN BY INDIVIDUAL ARGUMENTS
  if( bGivenStructData ) {
    if( theMaterialName == "" && theDensity <= 0 ) {
      G4Exception("changeStructure",
		  "Error",
		  FatalErrorInArgument,
		  "MATERIAL_NAME CANNOT BE EMTPY AND AT THE SAME TIME DENSITY < 0 ");
    }
    if( !bImageLimits ) {
      if( theStructureName == "" ) {
	G4Exception(theExeName,
		    "Error",
		    FatalErrorInArgument,
		    "STRUCTURE_NAME CANNOT BE EMTPY");
      }
      StructData stData;
      stData.stName = theStructureName;
      stData.mateName = theMaterialName;
      stData.density = theDensity;
      stData.bOnly = bStOnly;
      theStructData.push_back(stData);
    }
  }
  
  std::map<G4int,G4String> theStNames = theReaderMgr->GetStructIDsNames();
  std::map<G4int,G4String>::const_iterator itepv;
  //--- IF StrucData.stName IS "ALL" CREATE NEW STRUCT DATA'S
  size_t nSD = theStructData.size();
  for( size_t ii = 0; ii < nSD; ii++ ) {
    if( theStructData[ii].stName == "ALL" ) {
      for( itepv = theStNames.begin(); itepv != theStNames.end(); itepv++ ) {
	StructData stData;
	stData.stName = (*itepv).second;
	stData.mateName = theStructData[ii].mateName;
	stData.density = theStructData[ii].density;
	stData.bOnly = theStructData[ii].bOnly;
	theStructData.push_back(stData);
	if( DicomVerb(infoVerb) ) G4cout << " StructData search for stID " <<  (*itepv).first << " : " << stData.stName << G4endl;
      }
    }
  }
  //--- DELETE OLD StructData WITH stName = "ALL"
  std::vector<StructData>::iterator itesd;
  std::set< std::vector<StructData>::iterator > setAll;
  for( itesd = theStructData.begin(); itesd != theStructData.end(); itesd++ ) {
    if( (*itesd).stName == "ALL" ) {
      setAll.insert(itesd);
    }
  }
  std::set< std::vector<StructData>::iterator >::iterator itesvsd;
  for( itesvsd = setAll.begin(); itesvsd != setAll.end(); itesvsd++ ){
    theStructData.erase(*itesvsd);
  }      
  
  //----- CHECK THAT AT LEAST 1 StructData IS FILLED
  if( theStructData.size() == 0 && !bImageLimits ) {
    G4Exception("changeStructure",
		"Error",
		FatalErrorInArgument,
		"MISSING ARGUMENTS, NO STRUCTURE DATA");
  }

  //----- CHECK THAT THERE ARE NOT 2 StructData WITH THE SAME stID
  for( size_t i1 = 0; i1 < theStructData.size(); i1++ ) {
    for( size_t i2 = i1+1; i2 < theStructData.size(); i2++ ) {
      if( theStructData[i1].stID == theStructData[i2].stID ) {
	G4cerr << " !!! STRUCT " << i1 << " : " << theStructData[i1].stID << " " << theStructData[i1].stName << " " << theStructData[i1].density << " " << theStructData[i1].bOnly << G4endl;
	G4cerr << " !!! STRUCT " << i2 << " : " << theStructData[i2].stID << " " << theStructData[i2].stName << " " << theStructData[i2].density << " " << theStructData[i2].bOnly << G4endl;
 	G4Exception("changeStructure",
		    "",
		    FatalErrorInArgument,
		    "Two structures found with the same struct ID");
      }
    }
  }

  //----- GET PVS ID FROM PVS NAME
  //--- FIRST INITIALIZE mateID AND stID
  for( size_t ii = 0; ii < theStructData.size(); ii++ ) {
    theStructData[ii].mateID = -1; // INITIALISE mateIDs to -1
    theStructData[ii].stID = 0; // INITIALISE stIDs to 0
  }
  
  //---- ASSOCIATE stName TO stID FOR EACH STRUCT DATA
  if( bChangeMaterials ) {
    for( itepv = theStNames.begin(); itepv != theStNames.end(); itepv++ ) {
      G4String StName = (*itepv).second;
      for( size_t ii = 0; ii < theStructData.size(); ii++ ) {
	//	G4cerr << " LOOPING theStructData " << ii << " stID " << theStructData[ii].stID << " stName " <<  theStructData[ii].stName << G4endl; //GDEB
	if( StName == theStructData[ii].stName ) {
	  theStructData[ii].stID = (*itepv).first;
	  if( DicomVerb(infoVerb) ) G4cout << ii << " StructData found stID " <<  (*itepv).first << " : " << StName << G4endl;
	  break;
	}
      }
    }
    //--- CHECK THAT ALL StrucData.stName's HAVE BEEN FOUND
    for( size_t ii = 0; ii < theStructData.size(); ii++ ) {
      if( theStructData[ii].stID == 0 ) {
	G4cerr << " !!! theStructData " << ii << " stID " << theStructData[ii].stID << " stName " <<  theStructData[ii].stName << G4endl;
	G4Exception("changeStructure",
		    "",
		    FatalErrorInArgument,
		    (G4String("Structure name not found: ") + theStructData[ii].stName).c_str());
      }
    }
    
    //--- GET MATERIAL ID FROM MATERIAL NAME IN G4DCM FILE
    G4String theMaterialFound = "";
    std::vector<DicomVReaderImage*> vireaders = theReaderMgr->GetImageReaders(DRM_G4dcmCT); // there must be 1
    theG4dcmCTReader = static_cast<DicomReaderG4dcmCT*>(vireaders[0]);
    std::map<G4int,G4String> materialNames = theG4dcmCTReader->GetMaterialNames();
    //    G4cout << theG4dcmCTReader << " N_MATERIALS = " << materialNames.size() << G4endl; //GDEB
    std::map<G4int,G4String>::const_iterator ite;
    for( ite = materialNames.begin(); ite != materialNames.end(); ite++ ){
      //      G4cout << " LOOPING MATERIAL: " <<  (*ite).first << " = " <<  (*ite).second << G4endl; //GDEB
      G4String mateName = (*ite).second;
      for( size_t iis = 0; iis < theStructData.size(); iis++ ) {
	if( mateName == theStructData[iis].mateName ) {
	  theStructData[iis].mateID = (*ite).first;
	  if( DicomVerb(infoVerb) ) G4cout << " StructData mateID " << (*ite).first << " for material " << mateName << G4endl;
	}
      }
      if( bImageLimits ) {
	if( mateName == theMaterialName ) {
	  theMaterialID = (*ite).first;
	}
      }
    }
    //--- MATERIAL's IN StructData NOT FOUND IN G4DCM: MAKE NEW G4DCM MATERIAL
    for( size_t ii = 0; ii < theStructData.size(); ii++ ) {
      if( theStructData[ii].mateName != "" && theStructData[ii].mateID == -1 ) {
	std::map<G4int,G4String> materialNamesNow = theG4dcmCTReader->GetMaterialNames();
	G4int newMateID = (*(materialNamesNow.rbegin())).first + 1;
	theG4dcmCTReader->AddMaterialName(newMateID, theStructData[ii].mateName);
	theStructData[ii].mateID = newMateID;
	if( DicomVerb(infoVerb) ) G4cout << " newMaterial " << theStructData[ii].mateID << " : " << theStructData[ii].mateName << G4endl;
      }
    }
    if( bImageLimits ) {
      if( theMaterialID == -1 && bChangeMaterials ) {
	std::map<G4int,G4String> materialNamesNow = theG4dcmCTReader->GetMaterialNames();
	G4int newMateID = (*(materialNamesNow.rbegin())).first + 1;
	theG4dcmCTReader->AddMaterialName(newMateID, theMaterialName);
	theMaterialID = newMateID;
	if( DicomVerb(infoVerb) ) G4cout << " newMaterial " << theMaterialID << " : " << theMaterialName << G4endl;
      }
    }
  }
  //  G4cout << " NMATERIALS " << theMaterials.size() << G4endl; //GDEB

  //--- IF bImageLimits NO StructData IS NEEDED
  if( bImageLimits && theStructData.size() != 0 ) { 
    G4Exception(theExeName,
		"",
		FatalException,
		"If volume to be changed is given by min/max, no structure data should be defined");
  }
  
}

//---------------------------------------------------------------------------
void DCMChangeStructure::ChangeImages()
{
  if( !bImageLimits ) {
    ChangeImagesByStruct();
  } else {
    ChangeImagesByLimits();
  }
}
//---------------------------------------------------------------------------
void DCMChangeStructure::ChangeImagesByStruct()
{
  //--- LOOP TO VOXELS TO CHANGE MATERIAL OR DENSITY
  G4int nVoxels = theAnalyseImage->GetNoVoxels();
  //  G4cout << " DCMChangeStructure::ChangeImagesByStruct " << nVoxels << G4endl; //GDEB
  StructData stData = theStructData[0]; //GDEB
  //    G4cout << " STID SEARCHED " << stData.stID << G4endl; //GDEB
  for( G4int ii = 0; ii < nVoxels; ii++ ) {
    std::set<G4int> stIDList = theStructImage->GetIDList(ii);
    size_t nSTs = stIDList.size();
    //    if( stID != 0 ) G4cout << " PVSID " << ii << " : " << stID << " nSTs= " << nSTs << " : " << log(stID) << G4endl; //GDEB
    if( DicomVerb(debugVerb) && ii %100000 == 1 ) G4cout << " Looping Voxel " << ii << " nSTs " << nSTs << G4endl;

    /*    G4ThreeVector pos = theStructImage->GetPosition(ii);
    G4double posZ = pos.z(); //GDEB
    G4double posY = pos.y(); //GDEB
    if( posZ < -867 && posZ > -871 && posY < -100 && posY > -140 )   G4cout << " StIDLIST " << theStructImage->GetDataStr(ii) << " " << pos << stData.stName << " " << stData.mateName << G4endl; //GDEB 
    */
    for( std::set<G4int>::const_iterator itest = stIDList.begin(); itest != stIDList.end(); itest++ ) {
      size_t stid1 = *itest;
      //      if( stid != 0 )  G4cout << " PVSID1 " << stid1 << " = " << theStructureID << G4endl; //GDEB
      //--- CHECK St ID AND CHANGE MATERIAL AND/OR DENSITY
      for( size_t iis = 0; iis < theStructData.size(); iis++ ) {
	StructData stData = theStructData[iis];
	if( stData.density != -1 ) {
	  bChangeDensities = true;
	} else {
	  bChangeDensities = false;
	}
	if( stData.mateName != "" && stData.mateName != "NONE" ) {
	  bChangeMaterials = true;
	} else {
	  bChangeMaterials = false;
	}
	if( !bChangeDensities && !bChangeMaterials )  {
	  G4Exception(theExeName,
		      "",
		      FatalException,
		      "Not material nor material density to be changed for one StructData");
	}    
	//	std::cout << ii << " " << id1 << " " << iis << " stid1 == stData.stID " << stid1 << " == " << stData.stID << std::endl; //GDEB
	if( stid1 == stData.stID ) {
	  //	  std::cout << ii << " stid1 FOUND " << stid1 << " -> " << stData.mateID << " " <<  stData.bOnly << " " <<nSTs << " bImageG4dcmCT " << bImageG4dcmCT << std::endl; //GDEB
	  //	  if( posZ < -867 && posZ > -871 && posY < -100 && posY > -140 ) G4cout << " CHANGE StIDLIST " << stData.bOnly << " " << nSTs << " " << bImageG4dcmCT << " " << bChangeMaterials << G4endl; //GDEB 

	  if( stData.bOnly == false || nSTs == 1 ) {
	    if( bImageG4dcmCT ) {
	      if( bChangeMaterials ) theAnalyseImage->SetData(ii,stData.mateID);
	      if( bChangeDensities ) {
		//	  if( posZ < -867 && posZ > -871 && posY < -100 && posY > -140 ) G4cout << " CHANGE theMateDensImage " << ii << " : " << stData.density << G4endl; //GDEB 
		theMateDensImage->SetData(ii,stData.density);
	      }
	    } else {
	      theAnalyseImage->SetData(ii,stData.density);
	    }		
	    if( DicomVerb(debugVerb) ) G4cout << ii << " change mateID to " << stData.mateID << " dens " << stData.density << G4endl;	      
	  }
	}
      }
    }
  }

}

//---------------------------------------------------------------------------
void DCMChangeStructure::ChangeImagesByLimits()
{
  G4int nVoxels = theAnalyseImage->GetNoVoxels();
  size_t imNVoxelX = theAnalyseImage->GetNoVoxelsX();
  size_t imNVoxelY = theAnalyseImage->GetNoVoxelsY();
  size_t imNVoxelXY = imNVoxelX*imNVoxelY;
  G4double imMinX = theAnalyseImage->GetMinX();
  G4double imMinY = theAnalyseImage->GetMinY();
  G4double imMinZ = theAnalyseImage->GetMinZ();
  G4double imVoxDimX = theAnalyseImage->GetVoxelDimX();
  G4double imVoxDimY = theAnalyseImage->GetVoxelDimY();
  G4double imVoxDimZ = theAnalyseImage->GetVoxelDimZ();
  //  G4cout << " DCMChangeStructure::ChangeImagesByLimits " << nVoxels << G4endl; //GDEB
  for( G4int ii = 0; ii < nVoxels; ii++ ) {
    G4int ix = ii%imNVoxelX;
    G4int iy = (ii/imNVoxelX)%imNVoxelY;
    G4int iz = ii/imNVoxelXY;
    double voxXmin = imMinX + (ix+1)*imVoxDimX; // if minX line crosses voxel do not take it
    double voxXmax = imMinX + (ix)*imVoxDimX; // if maxX line crosses voxel do not take it
    double voxYmin = imMinY + (iy+1)*imVoxDimY; // if minY line crosses voxel do not take it
    double voxYmax = imMinY + (iy)*imVoxDimY; // if maxY line crosses voxel do not take it
    double voxZmin = imMinZ + (iz+1)*imVoxDimZ; // if minZ line crosses voxel do not take it
    double voxZmax = imMinZ + (iz)*imVoxDimZ; // if maxZ line crosses voxel do not take it
    /*      G4cout << ii << " " << ix << " CUT X " << voxXmin << " > " << theImageMinX << " && " << voxXmax << " < " << theImageMaxX << G4endl; //GDEB
      G4cout << ii << " " << iy << " CUT Y " << voxYmin << " > " << theImageMinY << " && " << voxYmax << " < " << theImageMaxY << G4endl; //GDEB
      G4cout << ii << " " << iz << " CUT Z " << voxZmin << " > " << theImageMinZ << " && " << voxZmax << " < " << theImageMaxZ << G4endl; //GDEB*/
    if( voxXmin > theImageMinX && voxXmax < theImageMaxX 
	&& voxYmin > theImageMinY && voxYmax < theImageMaxY
	&& voxZmin > theImageMinZ && voxZmax < theImageMaxZ ) {
      if( bChangeMaterials ) theAnalyseImage->SetData(ii,theMaterialID);
      if( bChangeDensities ) theMateDensImage->SetData(ii,theDensity);
      if( DicomVerb(debugVerb ) ) {
	G4cout << ii << " " << ix << " CUT X " << voxXmin << " > " << theImageMinX << " && " << voxXmax << " < " << theImageMaxX << G4endl; 
	G4cout << ii << " " << iy << " CUT Y " << voxYmin << " > " << theImageMinY << " && " << voxYmax << " < " << theImageMaxY << G4endl; 
	G4cout << ii << " " << iz << " CUT Z " << voxZmin << " > " << theImageMinZ << " && " << voxZmax << " < " << theImageMaxZ << G4endl; 
      }
    }
    //    G4cout << ii << " MATE ID  " << mateIDs[ii] << " " << mateDens[ii] << G4endl; //GDEB
  }
}

//---------------------------------------------------------------------------
void DCMChangeStructure::WriteNewImage()
{
  //--- DUMP TO FILE
  G4cout << " @@@ Dumping To Text File: " << theOutFN << G4endl;
  std::ofstream fout(theOutFN.c_str());

  if( bImageG4dcmCT ) {
    theG4dcmCTReader->DumpHeaderToTextFile(fout);
    theAnalyseImage->DumpHeaderToTextFile(fout);
  } else {
    theAnalyseImage->DumpHeaderToTextFile(fout);
  }
  theAnalyseImage->DumpDataToTextFile(fout);

  if( bImageG4dcmCT ) {
    theMateDensImage->DumpDataToTextFile(fout);
  }
  
  if( bImageG4dcmCT && theStructImage ) {
    theStructImage->DumpDataToTextFile(fout);   
    theG4dcmCTReader->DumpStructureList(fout);
  }

}

//---------------------------------------------------------------------------
void DCMChangeStructure::PrintHelp()
{
  G4cout << " -fDCMIn      G4 DICOM input file " << G4endl
	 << " -struct          DICOM structure name " << G4endl
	 << " -mate        new material name " << G4endl
	 << " -density     new material density " << G4endl
	 << " -stOnly      Only change voxels that belong exclusively to the structure or all (1/0). Default = 0" << G4endl
	 << " -stMateDens  DICOM structure name, new material name, new material density, stOnly " << G4endl
	 << " -fChangeList file with list of changes to be made (each line four words) " << G4endl
	 << " -fDCMOut     G4 DICOM ouput file " << G4endl
    	 << " -structureROIShift   Value to build different stID when a voxel belongs to several ROI: stID=Sum(pow(ROIShift,ii)*stID)(for ii=1,N_of_structures) (stID has to be smaller than pow(2,64)  Default is 100 " << G4endl
	 << " -minX        change all voxels with upper X boundary < minX" << G4endl
	 << " -maxX        change all voxels with lower X boundary > maxX" << G4endl
	 << " -minY        change all voxels with upper Y boundary < minY" << G4endl
	 << " -maxY        change all voxels with lower Y boundary > maxY" << G4endl
	 << " -minZ        change all voxels with upper Z boundary < minZ" << G4endl
	 << " -maxZ        change all voxels with lower Z boundary > maxZ" << G4endl
    	 << " -verb        sets the verbosity: test/debug/info/warning/error/silent" << G4endl
	 << " -h           prints the set of arguments " << G4endl
	 << " -help        prints the set of arguments " << G4endl;
}
  
