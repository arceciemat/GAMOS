#include "DCMDrawDICOM.hh"

#include "DICOM/DICOMBase/include/DicomVImage.hh"
#include "DICOM/DICOMBase/include/DicomMgr.hh"
#include "DICOM/DICOMBase/include/DicomParameterMgr.hh"
#include "DICOM/DICOMBase/include/DicomVerbosity.hh"
#include "DICOM/DICOMBase/include/DicomPolygonSet.hh"
#include "DICOM/DICOMBase/include/DicomVLineList.hh"

#include "DICOM/DICOMBase/include/DicomDrawerROOT.hh"
#include "DICOM/DICOMReaders/include/DicomReaderMgr.hh"
#include "DICOM/DICOMReaders/include/DicomReaderRTStruct.hh"
#include "DICOM/DICOMReaders/include/DicomReaderG4dcmCT.hh"
#include "DICOM/DICOMReaders/include/DicomReaderTextLines.hh"

#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "TStyle.h"

//---------------------------------------------------------------------------
DCMDrawDICOM::DCMDrawDICOM()
{
  theExeName = "drawDICOM";
  theDicomMgr = DicomMgr::GetInstance();
  theReaderMgr = DicomReaderMgr::GetInstance();
}

//---------------------------------------------------------------------------
void DCMDrawDICOM::ProcessArguments(int argc,char** argv)
{
  //----- PROCESS PARAMETERS
  if( argc == 1 ) {
    G4Exception(theExeName.c_str(),
		  "Error",
		FatalErrorInArgument,
		"NO ARGUMENT GIVEN ");
  } else if( argc == 2 ) {
    if( G4String(argv[1]) == "-h" || G4String(argv[1]) == "-help" ) {
       PrintHelp();
       exit(1);
    } else {
      DicomException(theExeName + ": ONLY ONE ARGUMENT: " + argv[1]);
    }
  } else {
    /*    if(argc%2 != 1) { 
      G4Exception(theExeName.c_str(),
		  "Error",
		  FatalErrorInArgument,
		  "WRONG NUMBER OF ARGUMENTS: THEY MUST BE -XX1 VAL_XX1 -XX2 VAL_XX2 ... ");
    }
    */
    
    for( G4int ii = 1; ii < argc; ii++ ){
      if( ii+1 >= argc ) {
	G4Exception(theExeName.c_str(),
		    "Error",
		    FatalErrorInArgument,
		    "WRONG NUMBER OF ARGUMENTS: THEY MUST BE -XX1 VAL_XX1 -XX2 VAL_XX2 ... ");
      }
      G4String argvstr = argv[ii];
      //      std::cout << ii << "ARGV " << argv[ii] << std::endl; //GDEB
      G4String argvstr1 = argv[ii+1];
      G4String argvName = argvstr.substr(1,999);
      if( argvstr == "-drawerType" ) {
	theParamMgr->AddParam( argvName + " " + argvstr1, PTstring );
	ii++;
      } else if( argvstr == "-palette"
		 || argvstr == "-bDrawXY"
		 || argvstr == "-bDrawXZ"
		 || argvstr == "-bDrawYZ" 
		 || argvstr == "-bDrawMateID" 
		 || argvstr == "-bDrawStructID" 
		 || argvstr == "-bDrawMateID1Fig" 
		 || argvstr == "-bDrawStructID1Fig" 
		 || argvstr == "-bDrawStruct1Fig"
		 || argvstr == "-bDrawIsodLines" 
		 || argvstr == "-overSurrounding" ) {
	theParamMgr->AddParam( argvName + " " + argvstr1, PTdouble );
	ii++;
      } else if( argvstr == "-doseVar" ) {
	theParamMgr->AddParam( argvName + " " + argvstr1, PTstring );
	ii++;
      } else if( argvstr == "-isodPerCents" ) {
	if ( !GmGenUtils::IsInteger(argv[ii+1]) ) {
	  G4Exception(theExeName.c_str(),
		      "Wrong argument",
		      FatalErrorInArgument,
		      (G4String("ARGUMENT: ") + argv[ii+1] + "   after -isodPerCents  should come an integer (number of fractions) ").c_str());
	}
	G4int nIPC = G4int(GmGenUtils::GetValue(argv[ii+1]));
	if( ii+1+nIPC >= argc ) {
	  G4Exception(theExeName.c_str(),
		      "Error",
		      FatalErrorInArgument,
		      "WRONG NUMBER OF ARGUMENTS AFTER -isodPerCents. THET MUST BE N_VALUES %VALUE_1 %VALUE_2 ... VALUE_N ");
	}
	
	for( int iipc = 1; iipc <= nIPC; iipc++ ) {
	  theIsodPerCents.push_back(GmGenUtils::GetValue(argv[ii+1+iipc]));
	}
	ii+=1+nIPC;

      } else if( argvstr == "-isodPerCentsAbs" ) {
	if ( !GmGenUtils::IsInteger(argv[ii+1]) ) {
	  G4Exception(theExeName.c_str(),
		      "Wrong argument",
		      FatalErrorInArgument,
		      (G4String("ARGUMENT: ") + argv[ii+1] + "   after -isodPerCentsAbs  should come an integer (number of fractions) ").c_str());
	}
	G4int nIPC = G4int(GmGenUtils::GetValue(argv[ii+1]));
	if( ii+1+nIPC >= argc ) {
	  G4Exception(theExeName.c_str(),
		      "Error",
		      FatalErrorInArgument,
		      "WRONG NUMBER OF ARGUMENTS AFTER -isodPerCentsAbs. THET MUST BE N_VALUES %VALUE_1 %VALUE_2 ... VALUE_N ");
	}
	
	for( int iipc = 1; iipc <= nIPC; iipc++ ) {
	  theIsodPerCentsAbs.push_back(GmGenUtils::GetValue(argv[ii+1+iipc]));
	}
	ii+=1+nIPC;

      } else if( argvstr == "-bFigureTitle" ) {
	theParamMgr->AddParam( argvName + " " + argvstr1, PTdouble );
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


  //------ StLines and IsodLines
  G4String drawLinesFN = theParamMgr->GetStringValue("fDrawLines","");
  theDicomMgr->ReadLineDrawingOptios( drawLinesFN );
  
  G4String theDrawerType = theParamMgr->GetStringValue("drawerType","ROOT");
  if( theDrawerType == "ROOT" ) {
    theDrawer = new DicomDrawerROOT();
  } else { 
    DicomException(theExeName + ": UNKNOWN DRAWER, IT CAN ONLY BE 'ROOT', WHILE IT IS "+ theDrawerType);
  }

  if( theParamMgr->GetNumericValue("overSurrounding",-1.) != -1. ) {
    theDrawer->SetOverSurrounding(theParamMgr->GetNumericValue("overSurrounding",-1));
  }
  
  G4int palette = G4int(theParamMgr->GetNumericValue("palette",55));
  gStyle->SetPalette(palette);
  /*  Int_t myPalette[5];
  myPalette[0] = 1;
  myPalette[1] = 2;
  myPalette[2] = 4;
  myPalette[3] = 6;
  myPalette[4] = 8;
   gStyle->SetPalette(5,myPalette);
  */
  
  bDrawXY = G4bool(theParamMgr->GetNumericValue("bDrawXY",1));
  bDrawXZ = G4bool(theParamMgr->GetNumericValue("bDrawXZ",1));
  bDrawYZ = G4bool(theParamMgr->GetNumericValue("bDrawYZ",1));
  bDrawMateID = G4bool(theParamMgr->GetNumericValue("bDrawMateID",0));
  bDrawStructID = G4bool(theParamMgr->GetNumericValue("bDrawStructID",0));
  bDrawMateID1Fig = G4bool(theParamMgr->GetNumericValue("bDrawMateID1Fig",0));
  bDrawStructID1Fig = G4bool(theParamMgr->GetNumericValue("bDrawStructID1Fig",0));
  bDrawStruct1Fig = G4bool(theParamMgr->GetNumericValue("bDrawStruct1Fig",0));
  bDrawIsodLines = G4bool(theParamMgr->GetNumericValue("bDrawIsodLines",0));

  if( theIsodPerCents.size() != 0 &&  theIsodPerCentsAbs.size() != 0 ) {
    G4Exception("DCMDrawDICOM::DCMDrawDICOM",
		"",
		FatalErrorInArgument,
		"You cannot give a list of IsodPerCents and another of IsodPerCentsAbs, as only one would be used");
  }

  if( theIsodPerCents.size() + theIsodPerCentsAbs.size() != 0 ) {
    if( !bDrawIsodLines ) {
      G4Exception("DCMDrawDICOM::DCMDrawDICOM",
		  "",
		  JustWarning,
		  "Giving a list of isodose per cents forces the drawing of isodose lines");
      bDrawIsodLines = 1;
    }
  } else {
    theIsodPerCents.push_back(90.);
    theIsodPerCents.push_back(50.);
    theIsodPerCents.push_back(10.);
  }

}

//---------------------------------------------------------------------------
void DCMDrawDICOM::CheckArguments()
{
  //----- CHECK CORRECT ARGUMENTS
  size_t nfct = theParamMgr->GetNofImageFiles("ANY");
  if( nfct == 0 ) {
    DicomException("THERE MUST BE ONE IMAGE FILE");
  }

  if( theParamMgr->GetNofImageFiles("fG4dcmCT") > 1 ) {
    DicomException("THERE MUST BE ONLY ONE IMAGE FILE OF TYPE G4dcmCT");
  }
  if( theParamMgr->GetNofImageFiles("fG4dcmNM") > 1 ) {
    DicomException("THERE MUST BE ONLY ONE IMAGE FILE OF TYPE G4dcmNM");
  }
  if( theParamMgr->GetNofImageFiles("fG4dcmDose") > 1 ) {
    DicomException("THERE MUST BE ONLY ONE IMAGE FILE OF TYPE G4dcmDose");
  }
  if( theParamMgr->GetNofImageFiles("fSqdose") > 1 ) {
    DicomException("THERE MUST BE ONLY ONE IMAGE FILE OF TYPE Sqdose");
  }
  if( theParamMgr->GetNofImageFiles("fRTDose") > 1 ) {
    DicomException("THERE MUST BE ONLY ONE IMAGE FILE OF TYPE RTDose");
  }
  if( theParamMgr->GetNofImageFiles("fNM") > 1 ) {
    DicomException("THERE MUST BE ONLY ONE IMAGE FILE OF TYPE NM");
  }
  if( theParamMgr->GetNofImageFiles("fRTStruct") > 1 ) {
    DicomException("THERE MUST BE ONLY ONE IMAGE FILE OF TYPE RTSTRUCT");
  }
  if( theParamMgr->GetNofImageFiles("fVOIStruct") > 1 ) {
    DicomException("THERE MUST BE ONLY ONE IMAGE FILE OF TYPE VOISTRUCT");
  }
  if( theParamMgr->GetNofImageFiles("fRTStruct") + theParamMgr->GetNofImageFiles("fVOIStruct") > 1 ) {
    DicomException("THERE MUST BE ONLY ONE IMAGE FILE OF TYPE RTSTRUCT OR VOISTRUCT");
  }
  if( theParamMgr->GetNofImageFiles("fInterfile") > 1 ) {
    DicomException("THERE MUST BE ONLY ONE IMAGE FILE OF TYPE Sqdose");
  }
  if( (bDrawMateID || bDrawMateID1Fig) && theParamMgr->GetNofImageFiles("fG4dcmCT") != 1 ) {
    DicomException("TO DRAW MATEID THERE MUST BE ONE IMAGE FILE OF TYPE G4dcmCT");
  }
  if( bDrawStructID || bDrawStructID1Fig ) {
    if( theParamMgr->GetNofImageFiles("fG4dcmCT") != 1 && theDicomMgr->GetNofImages(DIM_G4dcmCT_StructID) != 1 ) { 
      DicomException("TO DRAW STRUCTID THERE MUST BE ONE IMAGE FILE OF TYPE G4dcmCT WITH STRUCTURES");
    }
  }
	 
  if(bDrawStruct1Fig && (theParamMgr->GetNofImageFiles("fRTStruct") + theParamMgr->GetNofImageFiles("fVOIStruct") != 1) ) {
    DicomException("TO DRAW STRUCTURES IN ONE FIGURE THERE MUST BE ONE IMAGE FILE OF TYPE RTSTRUCT");
  }

}

//---------------------------------------------------------------------------
void DCMDrawDICOM::ReadFilesAndGetImages()
{
  //--- READ AND BUILD IMAGES
  theReaderMgr = DicomReaderMgr::GetInstance();
  //  theReaderMgr->ProcessData();
  theReaderMgr->CreateReaders();
  theReaderMgr->SetCTOnlyHU(true);
  theReaderMgr->CreateImages();

  std::vector<DicomVImage*> images = theReaderMgr->GetImages(); //?

  theDicomMgr->OperateAll();
}

//---------------------------------------------------------------------------
void DCMDrawDICOM::DrawImages()
{
  //  std::map<DicomVImage*, DicomPolygonSet*> theIsodosePolygonSets; // each image has a set of polygons from isodoses
  DicomPolygonSet* theStPolygonSet = 0;
  //  DicomPolygonSet* theIsoDPolygonSet = 0;
  std::vector<DicomVLineSet*> theVLineSets;
  
  DicomReaderRTStruct* RTFile = 0;
  //--- Get struct lines
  std::vector<DicomVReader*> VRTFiles = theReaderMgr->GetReaders(DRM_RTStruct, false);
  //  G4cout << " N RTStruct " << VRTFiles.size() << G4endl; //GDEB
  if( VRTFiles.size() > 1 ) {
    G4Exception("DCMDrawDICOM::DrawImages",
		"",
		FatalException,
		"Only one RTStruct is allowed");
  } else if( VRTFiles.size() == 1 ) {
    RTFile = dynamic_cast<DicomReaderRTStruct*>(VRTFiles[0]);
    theStPolygonSet = RTFile->GetPolygonSet();
    theVLineSets.push_back(theStPolygonSet);
    // GDEB
    /*    G4cout << " DCMDrawDICOM::DrawImages ( GOT POLYGON SET " << theStPolygonSet->GetName() << " Nlists " <<  theStPolygonSet->GetLineLists().size() << G4endl;
    for( size_t ii = 0; ii <  theStPolygonSet->GetLineLists().size(); ii++ ) {
      DicomVLineList* lineList =  theStPolygonSet->GetLineLists()[ii];
      G4cout << ii << " POLYGON LINELIST " << lineList->GetName() << " Nlines "<< lineList->GetLines().size() << G4endl; 
      for( size_t jj = 0; jj < lineList->GetLines().size(); jj++ ) {
	DicomVLine* line = lineList->GetLines()[jj];
	G4cout << jj << " POLYGON LINE " << line->GetName() << " Nlines "<< line->GetPoints().size() << G4endl;
      }
      }*/
  }

  //?    std::vector<DicomVImage*> imagesSt = theReaderMgr->GetImages(DRM_RTStruct);
 //t  if( bDrawOnlySt )     RTFile->FindVoxelsInXY( images[0] );    DicomVImage* rtImage = RTFile->GetStructImage() const {
  //--- Get isodose lines
  //  G4cout << " N PolySets " << theVLineSets.size() << G4endl; //GDEB
  //  //--- at least one (empty polygon set)
  //  if( theVLineSets.size() == 0 ) {
  //    theVLineSets.push_back((DicomPolygonSet*)(0));
  //  }
  std::vector<DicomVImage*> images = theReaderMgr->GetImages();

  //---- Replace MateDens by MateID or StructID if requested
  if( bDrawMateID || bDrawStructID )  {
    G4bool bFound = false;
    std::vector<DicomVReader*>::iterator iteCT;
    // loop to readers
    VRTFiles = theReaderMgr->GetReaders(DRM_G4dcmCT, false);
    for( std::vector<DicomVReader*>::iterator ite = VRTFiles.begin(); ite != VRTFiles.end(); ite++ ) {
      DicomVReader* reader = *ite;
      //      G4cout << " LOOPING READERS " << reader->GetModalityStr(reader->GetModality()) << " " << reader->GetModality() << G4endl;  //GDEB
      if( reader->GetModality() == DRM_G4dcmCT ) {
	iteCT = ite;
	if( bDrawMateID ) {
	  std::vector<DicomVImage*> imgs = theDicomMgr->GetImages(DIM_G4dcmCT_MateID,false);
	  for( size_t iii = 0; iii < imgs.size(); iii++ ) {
	    bFound = true;
	    images.push_back(imgs[iii]);
	    /*	    std::vector<G4double>* data = imgs[iii]->GetData();
	    G4int nData = imgs[iii]->GetNoVoxels();
	    for( size_t iid = 0; iid < nData; iid++ ) {
	      G4cout << " MATEID DATA " << data->at(iid) << " " << iid << G4endl; //GDEB
	      } */
	  }
	}
	if( bDrawStructID ) {
	  //---- Build a new image, converting ID (nn:mm:ll:..) to 1 integer
	  /*	  std::vector<DicomVReader*> VG4dcmCTFiles = theReaderMgr->GetReaders(DRM_G4dcmCT,false);
	  DicomReaderG4dcmCT* g4dcmReader = dynamic_cast<DicomReaderG4dcmCT*>(VG4dcmCTFiles[0]);
	  DicomVImageStr* imgStID;
	  if( g4dcmReader->HasStructID()  ) {
	    imgStID = g4dcmReader->GetStructIDImage();
	    }*/
	  std::vector<DicomVImage*> imgs = theDicomMgr->GetImages(DIM_G4dcmCT_StructID,false);
	  DicomVImage* img = imgs[0];
	  DicomVImageStr* imgStID = dynamic_cast<DicomVImageStr*>(img); 
	  DicomVImage* imgStIDDraw = new DicomVImage(imgStID, imgStID->GetName(), imgStID->GetModality(), imgStID->GetOrientation() );
	  size_t nData = imgStID->GetNoVoxels();
	  //	  std::vector<G4String>* dataStr = imgStID->GetDataStr();
	  //	  G4cout << " NVOXELS " << nData << " " << imgStID->GetNoVoxelsX() << " " << imgStID->GetNoVoxelsY() << " " << imgStID->GetNoVoxelsZ() << " " << dataStr->size() << G4endl; //GDEB
	  std::vector<G4double>* data = imgStIDDraw->GetData();
	  for( size_t iid = 0; iid < nData; iid++ ) {
	    std::set<G4int> ists = imgStID->GetIDList(iid );
	    /*	    G4cout << "DATASTR " << dataStr->at(iid) << " " << iid << G4endl; //GDEB
		    G4cout << " ISTS " << ists.size() << " " << *(ists.begin()) << " " << iid << G4endl; //GDEB */
	    if( ists.size() > 1 ) {
	      G4Exception("DCMDrawDICOM",
			  "",
			  FatalException,
			  "It is not possible to draw Struct ID images if there are more than one structures in one voxel, you may draw 1 figure per Structure ID (-bDrawStructID1Fig 1)");
	    }
	    data->at(iid) = *(ists.begin());
	  }
	  //	  G4cout << " LOOPING READERS N IMAGES StructID " << imgs.size() << G4endl; //GDEB
	  bFound = true;
	  images.push_back(imgStIDDraw);
	}
      }
    }
    if( !bFound ) {
      if( bDrawMateID ) {
      G4Exception(theExeName.c_str(),
		  "",
		  FatalException,
		  "No MateID image found while -bDrawMateID 1");
      }
      if( bDrawStructID ) {
            G4Exception(theExeName.c_str(),
		  "",
		  FatalException,
		  "No StructID image found while -bDrawStrucID");
      }
    } else {
      //--- delete MateDens image
      for( std::vector<DicomVImage*>::iterator itevi = images.begin(); itevi != images.end(); itevi++ ) {
	if( (*itevi)->GetModality() == DIM_G4dcmCT_MateDens ) {
	  images.erase(itevi);
	  break;
	}
      }
    }
  }

  //---- Replace Sqdose by doseSq, doseErr, doseErrRel
  G4String doseVar = theParamMgr->GetStringValue("doseVar","");
  if( doseVar != "" ) {
    //    G4cout << " DOSEVAR " << doseVar << G4endl; //GDEB
      // loop to readers
    std::vector<DicomVImage*> VSqdoseImages = theDicomMgr->GetImages(DIM_Sqdose, false);
    if( VSqdoseImages.size() == 0 ) {
      G4Exception("DCMDrawDicom::DrawImages",
		  "",
		  JustWarning,
		  "Using -doseVar but there is no sqdose file");
    }
    //--- delete sqdose images from list
    std::vector<DicomVImage*>::iterator itesi;
    std::vector<DicomVImage*> imgs;
    if( doseVar != "dose" ) {
      for( itesi = VSqdoseImages.begin(); itesi != VSqdoseImages.end(); itesi++ ) {
	for( std::vector<DicomVImage*>::iterator itei = images.begin(); itei != images.end(); itei++ ) {
	  if( *itei == *itesi ) {
	    images.erase(itei);
	    break;
	  }
	}
      }
    }
    //--- add new dose images to list
    if( doseVar == "doseSq" ) {
      imgs = theDicomMgr->GetImages(DIM_Sqdose_sq,false);
    } else if( doseVar == "doseErr" ) {
      imgs = theDicomMgr->GetImages(DIM_Sqdose_err,false);
    } else if( doseVar == "doseErrRel" ) {
      imgs = theDicomMgr->GetImages(DIM_Sqdose_errRel,false);
    } else if( doseVar == "dose" ) {
    } else {
      G4Exception("DCMDrawDICOM::DrawImages",
		  "",
		  FatalException,
		  ("Argument after -doseVar  must be dose/doseSq/doseErr/doseErrRel , while it is " + doseVar).c_str());
    }
    //    G4cout << " DOSEVAR N IMAGES " << imgs.size() << G4endl; //GDEB
    for( size_t iii = 0; iii < imgs.size(); iii++ ) {
      images.push_back(imgs[iii]);
    }
  }

  //----- DRAW IMAGES
  for( size_t ii = 0; ii < images.size(); ii ++ ) {
    DrawImage(images[ii],theVLineSets);
  }

  //---- Print 1 image per MateID
  if( bDrawMateID1Fig ) {
    G4bool bFound = false;
    VRTFiles = theReaderMgr->GetReaders(DRM_G4dcmCT, false);
    for( std::vector<DicomVReader*>::iterator ite = VRTFiles.begin(); ite != VRTFiles.end(); ite++ ) {
      DicomVReader* reader = *ite;
      std::map<size_t,DicomVImage*> theMateIDImages;
      if( reader->GetModality() == DRM_G4dcmCT ) {
	DicomReaderG4dcmCT* readerCT = dynamic_cast<DicomReaderG4dcmCT*>(reader);
	DicomVImage* imageMateID = readerCT->GetMateIDImage();
	//	std::map<G4int,G4String> stNames = readerCT->GetStructNames();
	theMateIDImages.clear();
	std::vector<G4double> imgData = *(imageMateID->GetData());
	size_t nVox = imageMateID->GetNoVoxels();
	for( size_t ii = 0; ii < nVox; ii++) {
	  G4int mateID = G4int(imgData[ii]);
	  std::map<size_t,DicomVImage*>::const_iterator iteimg = theMateIDImages.find(mateID);
	  DicomVImage* img1 = 0;	      
	  if( iteimg == theMateIDImages.end() ) { 
	    img1 = new DicomVImage(imageMateID,imageMateID->GetName()+"_"+readerCT->GetMaterialName(mateID),DIM_G4dcmCT_MateID);
	    img1->SetData(new std::vector<G4double>(nVox));
	    theMateIDImages[mateID] = img1;
	  } else {
	    img1 = (*iteimg).second;
	  }
	  if( mateID == 0 ) mateID = 999; // else MATEID=0 would be a bla
	  img1->SetData(ii,mateID);
	  bFound = true;;
	} 
	for( std::map<size_t,DicomVImage*>::const_iterator iteid = theMateIDImages.begin(); iteid != theMateIDImages.end(); iteid++ ) {
	  DrawImage((*iteid).second,theVLineSets);
	}	
      }
    }
    if( !bFound ) {
      G4Exception(theExeName.c_str(),
		  "",
		  FatalException,
		  "No MateID image found while -bDrawMateID1Fig 1");
    }    
  }

  //---- Print 1 image per StructID, with struct lines superimposed
  if( bDrawStructID1Fig ) {
    VRTFiles = theReaderMgr->GetReaders(DRM_G4dcmCT, false);
    for( std::vector<DicomVReader*>::iterator ite = VRTFiles.begin(); ite != VRTFiles.end(); ite++ ) {
      DicomVReader* reader = *ite;
      std::map<size_t,DicomVImage*> theStructIDImages;
      if( reader->GetModality() == DRM_G4dcmCT ) {
	DicomReaderG4dcmCT* readerCT = dynamic_cast<DicomReaderG4dcmCT*>(reader);
	DicomVImageStr* imageStructID = readerCT->GetStructIDImage();

	std::map<G4int,G4String> stNames = readerCT->GetStructNames();
	if( !imageStructID ) {
	  DicomException("No StructID image found while -bDrawStructID1Fig 1");
	}    
	theStructIDImages.clear();
	size_t nVox = imageStructID->GetNoVoxels();
	for( size_t ii = 0; ii < nVox; ii++) {
	  //	  if( ii%100000 == 1 ) G4cout << ii << " IDST1 " << nVox << " " << imageStructID->GetData(ii) << " = " << imgData->at(ii) << G4endl; //GDEB
	  //	  G4cout << imageStructID << " " << ii << " PREVSTDATA " << " : " << imageStructID->GetData(ii) << G4endl;
	  std::set<G4int> ists = imageStructID->GetIDList(ii);
	  
	  for( std::set<G4int>::const_iterator itest = ists.begin(); itest != ists.end(); itest++ ) {
	    size_t ist1 = *itest;
	    if( ist1 == 0 ) continue;
	    std::map<size_t,DicomVImage*>::const_iterator iteimg = theStructIDImages.find(ist1);
	    DicomVImage* img1 = 0;	      
	    if( iteimg == theStructIDImages.end() ) {
	      G4cout << " NEW theStructIDImages " << imageStructID << G4endl;
	      img1 = new DicomVImage(imageStructID,imageStructID->GetName()+"_"+readerCT->GetStructName(ist1),DIM_G4dcmCT_StructID);
	      img1->SetData(new std::vector<G4double>(nVox));
	      theStructIDImages[ist1] = img1;
	    } else {
	      img1 = (*iteimg).second;
	    }
	    img1->SetData(ii,ist1);
	    //	    if( ist1 != 0 ) G4cout << img1 << " SETDATA " << ii << " = " << ist1 << G4endl; //GDEB
	  }
	}
	if( theVLineSets.size() > 0 ) {
	  DicomVLineSet* lineSet = theVLineSets[0];
	  for( std::map<size_t,DicomVImage*>::const_iterator iteid = theStructIDImages.begin(); iteid != theStructIDImages.end(); iteid++ ) {
	    //	    G4cout << (*iteid).second << " STDATA DrawImage " << stNames[(*iteid).first] << " " << (*iteid).first << " " <<(*iteid).second->GetName() << G4endl; //GDEB
	    size_t stID = (*iteid).first;
	    DicomVLineSet* lnSet = new DicomVLineSet(lineSet->GetName(),lineSet->GetOrientation());
	    lnSet->AddLineList(lineSet->GetLineList(stID));
	    std::vector<DicomVLineSet*> lnSetV;
	    lnSetV.push_back(lnSet);
	    DrawImage((*iteid).second,lnSetV,"."+stNames[stID]);
	    //	  delete pgSet;
	  }
	} else {
	  for( std::map<size_t,DicomVImage*>::const_iterator iteid = theStructIDImages.begin(); iteid != theStructIDImages.end(); iteid++ ) {
	    size_t stID = (*iteid).first;
	    DrawImage((*iteid).second,std::vector<DicomVLineSet*>(),"."+stNames[stID]);
	  }
	}
      }
    }
  }
  
  //---- Print 1 image per Struct
  if( bDrawStruct1Fig ) {
    //    G4cout << " bDrawStruct1Fig " << bDrawStruct1Fig << G4endl; //GDEB
    //--- Divide theVLineSets
    if( theVLineSets.size() != 1 ) {
      DicomException("More than 1 polygon set when drawing 1 struct per file");
    }
    DicomVLineSet* lineSet = theVLineSets[0];
    DicomVImageStr* structImg = dynamic_cast<DicomVImageStr*>(theDicomMgr->GetImages(DIM_G4dcmCT_StructID)[0]);
    DicomVImage* blankImg = new DicomVImage(structImg,"StructIDLine",DIM_RTStruct);
    std::vector<DicomVLineList*> lineLists = lineSet->GetLineLists();
    for( size_t ii = 0; ii < lineLists.size(); ii++ ) {
      DicomVLineList* lineList = lineLists[ii];
      size_t stID = lineList->GetNumber();
      //      G4cout << " bDrawStruct1Fig " << lineList->GetName() << " " << lineList->GetNumber() << G4endl; //GDEB
      DicomVLineSet* lnSet = new DicomVLineSet(lineSet->GetName(),lineSet->GetOrientation());
      if( lineSet->GetLineList(stID)->GetLines().size() == 0 ) continue;
      //      pgSet->AddLineList(polygonSet->GetLineList(stID));
      lnSet->AddLineList(lineList);
      std::vector<DicomVLineSet*> lnSetV;
      lnSetV.push_back(lnSet);      
      DrawImage(blankImg,lnSetV,"."+lineList->GetName());
      //    delete pgSet;
    }

  }

}

//-------------------------------------------------------------------
void DCMDrawDICOM::DrawImage(DicomVImage* image, std::vector<DicomVLineSet*> lineSets, G4String extraFileName)
{
  //  G4cout << " DrawImage DrawXY " << bDrawXY << G4endl; //GDEB
  for( size_t ils = 0; ils < lineSets.size(); ils++ ) { // only polygons from RTStruct are included 
    //    DicomVLineSet* lineSet = lineSets[ils];
    // GDEB
    /*    G4cout << " DCMDrawDICOM::DrawImage( BEFORE DRAW POLYGON SET " << lineSet->GetName() << " Nlists " <<  lineSet->GetLineLists().size() << G4endl;
    for( size_t ii = 0; ii <  lineSet->GetLineLists().size(); ii++ ) {
      DicomVLineList* lineList =  lineSet->GetLineLists()[ii];
      G4cout << ii << " POLYGON LINELIST " << lineList->GetName() << " Nlines "<< lineList->GetLines().size() << G4endl; 
      for( size_t jj = 0; jj < lineList->GetLines().size(); jj++ ) {
	DicomVLine* line = lineList->GetLines()[jj];
	G4cout << jj << " POLYGON LINE " << line->GetName() << " Nlines "<< line->GetPoints().size() << G4endl;
      }
      } */
    lineSets[ils] = new DicomPolygonSet(dynamic_cast<DicomPolygonSet*>(lineSets[ils]), image, lineSets[ils]->GetOrientation());
    if( DicomVerb(debugVerb) ) {
      G4cout << " DCMDrawDICOM::DrawImage( DRAW POLYGON SET " << lineSets[ils]->GetName() << " Nlists " <<  lineSets[ils]->GetLineLists().size() << G4endl;
      for( size_t ii = 0; ii <  lineSets[ils]->GetLineLists().size(); ii++ ) {
	DicomVLineList* lineList =  lineSets[ils]->GetLineLists()[ii];
	G4cout << ii << " POLYGON LINELIST " << lineList->GetName() << " Nlines "<< lineList->GetLines().size() << G4endl; 
	for( size_t jj = 0; jj < lineList->GetLines().size(); jj++ ) {
	  DicomVLine* line = lineList->GetLines()[jj];
	  G4cout << jj << " POLYGON LINE " << line->GetName() << " Nlines "<< line->GetPoints().size() << G4endl;
	  for( size_t ipt = 0; ipt < line->GetPoints().size(); ipt++ ) {
	    G4cout << line << " " << ipt << " POLYGON POINT " << line->GetPoints()[ipt] << G4endl;
	  }
	}
      }
    }
    //    delete lineSet;
  }
  
  std::vector<DicomVReader*> VRTFiles = theReaderMgr->GetReaders(DRM_TextLines, false);
  for( size_t ii = 0; ii < VRTFiles.size(); ii++ ) {
    DicomReaderTextLines* lineReader = dynamic_cast<DicomReaderTextLines*>(VRTFiles[ii]);
    //    G4cout << " DCMDrawDICOM::DrawImage( GetReaders(DRM_TextLines lineReader "<< lineReader << G4endl; //GDEB
    lineSets.push_back( lineReader->GetLineSet() );
  }
  //@@@ build isodose values
  std::vector<G4double> isodoseValues;
  if( theIsodPerCents.size() != 0 ) {
    G4double maxValue = image->GetMaxValue();
    for( size_t iisod = 0; iisod < theIsodPerCents.size(); iisod++ ) {
      isodoseValues.push_back(theIsodPerCents[iisod]/100.*maxValue);
      if( DicomVerb(debugVerb) ) G4cout << " DCMDrawDICOM::DrawImage isodose " << iisod << " = " << isodoseValues[iisod]<< " = " << theIsodPerCents[iisod] << " /100* " << maxValue << std::endl;
    }
  }
  if( theIsodPerCentsAbs.size() != 0 ) {
    for( size_t iisod = 0; iisod < theIsodPerCentsAbs.size(); iisod++ ) {
      isodoseValues.push_back(theIsodPerCentsAbs[iisod]);
      if( DicomVerb(debugVerb) ) G4cout << " DCMDrawDICOM::DrawImage isodose " << iisod << " = " << isodoseValues[iisod] << std::endl;
    }
  }
  if( bDrawXY ) {
    if( bDrawIsodLines ) {
    //    if( bDrawIsodLines && image->IsDose() ) {      
      image->BuildIsodLinesXY(isodoseValues);
      lineSets.push_back(image->GetIsodosesXY());
    }
    theDrawer->DrawXY(image,lineSets,extraFileName);
  }
  if( bDrawXZ ) {
    if( bDrawIsodLines && image->IsDose() ) {
      image->BuildIsodLinesXZ(isodoseValues);
      lineSets.push_back(image->GetIsodosesXZ());
    } 
    theDrawer->DrawXZ(image,lineSets,extraFileName);

  }
  if( bDrawYZ ) {
    if( bDrawIsodLines && image->IsDose() ) {
      image->BuildIsodLinesYZ(isodoseValues);
      lineSets.push_back(image->GetIsodosesYZ());
    } 
    theDrawer->DrawYZ(image,lineSets,extraFileName);
  }

}
  
//-------------------------------------------------------------------
void DCMDrawDICOM::PrintHelp()
{
  ((DicomParameterMgr*)(DicomParameterMgr::GetInstance()))->PrintHelpInputFiles();
  G4cout << " -readerType READER_TYPE   type of reader (default = ROOT) " << G4endl
	 << " -bDrawMateID1Fig 0/1   option to draw one figure per each material in CT images (default = 1)" << G4endl
	 << " -bDrawStructID1Fig 0/1   option to draw figures with only structure IDs (from G4dcmCT), one figure per structure ID (default = 1)" << G4endl
	 << " -bDrawStruct1Fig 0/1   option to draw figures with only structures polygons (from RTStruct), one figure per structure ID (default = 1)" << G4endl
	 << " -bDrawMateID 0/1   option to draw colors by material name in CT images, instead of material density (default = 1)" << G4endl
	 << " -bDrawStructID 0/1   option to draw colors by structure ID in CT images, instead of material density (default = 1)" << G4endl
	 << " -bDrawXY 0/1   option to draw XY plane slices with structures superimposed (default = 1)" << G4endl
	 << " -bDrawXZ 0/1   option to draw XZ plane slices (default = 1)" << G4endl
	 << " -bDrawYZ 0/1   option to draw YZ plane slices (default = 1)" << G4endl
	 << " -palette NUMBER   choose integer code of color palette (for codes see https://root.cern.ch/doc/master/classTColor.html) " << G4endl
	 << " -paletteMin VALUE   set minimum of palette" << G4endl
	 << " -paletteMax VALUE   set maximum of palette" << G4endl
	 << " -nPixelsX VALUE   number of pixels used for each figure in the X axis. Default is 1000. " << G4endl
	 << " -nPixelsY VALUE   number of pixels used for each figure in the Y axis. Default is 1000. " << G4endl
	 << " -nPixelsZ VALUE   number of pixels used for each figure in the Z axis. Default is 1000. " << G4endl
	 << " -imageFormat FORMAT   available format are jpg (default), png, eps, ps, pdf, svg, xpm, jpg and tiff " << G4endl
	 << " -fDrawLines FILE_NAME   file where line format is defined (color, style, width, ...) " << G4endl
  	 << " -rotateXY DEGREES   figures are rotated by VAL in XY plane (=around Z axis)" << G4endl
	 << " -printMax  0/1   prints the maximum value of any voxel in each image together with the X/Y/Z voxel id's and X/Y/Z position of the voxel center" << G4endl
	 << " -printValueAtPoint POINT_X POINT_Y POINT_Z*: prints the value at the voxel in each image corresponding to the POINT together with the X/Y/Z voxel id's and X/Y/Z position of the voxel center" << G4endl
	 << " -bDrawIsodLines 1/0   draw isodose lines or not (default)" << G4endl  
	 << " -isodPerCents N_VALUES %VALUE_1 %VALUE_2 ...    Define the isodose percentages (w.r.t. to maximum value in image). Default is 90 50 10" << G4endl
	 << " -isodPerCentsAbs N_VALUES %VALUE_1 %VALUE_2 ...    Define the isodose percentages (in absolute values). Default is 90 50 10" << G4endl
	 << " -overSurrounding FACTOR   limit voxel values to FACTOR*average values of surrounding voxels" << G4endl
	 << " -bFigureTitle 0/1 (1)   print the figure title in figure files " << G4endl; 
}
  
