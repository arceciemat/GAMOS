#include "DCMGetGammaIndex.hh"

#include "DICOM/DICOMBase/include/DicomMgr.hh"
#include "DICOM/DICOMBase/include/DicomParameterMgr.hh"
#include "DICOM/DICOMBase/include/DicomVerbosity.hh"
#include "DICOM/DICOMReaders/include/DicomReaderMgr.hh"
#include "DICOM/DICOMReaders/include/DicomReaderRTStruct.hh"
#include "DICOM/DICOMReaders/include/DicomReaderSqdose.hh"
#include "DICOM/DICOMReaders/include/DicomReaderRTDose.hh"
#include "DICOM/DICOMReaders/include/DicomReaderG4dcmDose.hh"
#include "DICOM/DICOMReaders/include/DicomReaderG4dcmCT.hh"
#include "DICOM/DICOMReaders/include/DicomReaderTextLines.hh"
#include "DICOM/DICOMBase/include/DicomOperMultByNumber.hh"
#include "DICOM/DICOMBase/include/DicomVLineSet.hh"
#include "DICOM/DICOMBase/include/DicomVLineList.hh"
#include "DICOM/DICOMBase/include/DicomPolygonSet.hh"
#include "DICOM/DICOMBase/include/DicomPolygonList.hh"
#include "DICOM/DICOMBase/include/DicomVDrawer.hh"
#include "DICOM/DICOMBase/include/DicomDrawerROOT.hh"
#include "DICOM/DICOMBase/include/DicomOperIntersectStruct.hh"

#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "TStyle.h"
#include <iostream>
#include <set>
#include <map>

//------------------------------------------------------------------------
DCMGetGammaIndex::DCMGetGammaIndex()
{
  Initialise(); 
  theParamMgr = (DicomParameterMgr*)(DicomParameterMgr::GetInstance());
  theReaderMgr = DicomReaderMgr::GetInstance();
  theReaderMgr->SetMergeRTReaders( false );
  theDicomMgr = DicomMgr::GetInstance();  Initialise();
  theStructImage = 0;
  theGammaOverCut = 1.;
}

//------------------------------------------------------------------------
void DCMGetGammaIndex::Initialise()
{
  theDefaultPercent = 2.;
  theDefaultDistance = 3.;
  bDistanceByVoxel = false; //??
  
  theExeName = "getGammaIndex";
  theNormType = GIDN_None;
  //  bMaxAtPoint = false;
  
  bPerSt = false; // Print histos per Structure
  bDiffPerSt = false; // Dose difference per Structure
  bOnly1St = false; // Dose difference per Structure
  theOnly1StName = "";
  bGetGammaIndex = true; // Dose difference per Structure

  bUseMateID = false;
  bUseStructID = false;

  thePerCentType = GIPCT_None;
}

//---------------------------------------------------------------------------
void DCMGetGammaIndex::ProcessArguments(int argc,char** argv)
{
  std::vector<float> percents;
  std::vector<float> distances;
  
  //--------- PROCESS ARGUMENTS
  if( argc == 1 ) {
    G4Exception(theExeName.c_str(),
		  "",
		FatalErrorInArgument,
		"NO ARGUMENT GIVEN ");
  } else if( argc == 2 ) {
    if( G4String(argv[1]) == "-h" || G4String(argv[1]) == "-help" ) {
       PrintHelp();
       exit(1);
    } else {
      G4Exception(theExeName.c_str(),
		  "",
		  FatalErrorInArgument,
		  (G4String("ONLY ONE ARGUMENT: ") + argv[1]).c_str());
    }
  } else {
    G4bool bAtPoint = false;
    for( G4int ii = 1; ii < argc; ii++ ){
      if(G4String(argv[ii]) == "atPoint" ) {
	bAtPoint = true;
	break;
      }
    }

    if((argc+bAtPoint)%2 != 1) { 
       G4Exception(theExeName.c_str(),
		  "Error",
		  FatalErrorInArgument,
		  "WRONG NUMBER OF ARGUMENTS: THEY MUST BE -XX1 VAL_XX1 -XX2 VAL_XX2 ... ");
		  } 
    
    for( G4int ii = 1; ii < argc; ii++ ){
      G4String argvstr = argv[ii];
      G4String argvstr1 = argv[ii+1];
      G4String argvName = argvstr.substr(1,999);
      if( argvstr == "-bPerSt" ||
	  argvstr == "-bDistanceByVoxel" ||
	  argvstr == "-nErrorSigmas" ||
	  argvstr == "-bInterpolateVoxels" ||
	  //	  argvstr == "-maxDistVoxel" ||
	  //	  argvstr == "-maxAtPointFactor" ||
	  argvstr == "-maxGammaValue" || 
	  argvstr == "-pointExtensionN" ||
	  //	  argvstr == "-bGetGammaIndex" ||
	  //	  argvstr == "-bDiffPerSt" ||
	  //	  argvstr == "-fOutDiffPerSt" ||
	  argvstr == "-bUseMateID" ||
	  argvstr == "-bUseStructID" ||
	  argvstr == "-bHisto1D" ||	  
	  argvstr == "-bDrawXY"  ||	  
	  argvstr == "-bDrawXZ"  ||	  
	  argvstr == "-bDrawYZ"  ||	  
	  argvstr == "-bOnlyDiff" ) {  // gamma index=only diff (perCent), no distance
	theParamMgr->AddParam( argvName + " " + argvstr1, PTdouble );
	ii++;
      } else if( argvstr == "-only1St" ) {
	theParamMgr->AddParam( argvName + " " + argvstr1, PTstring );
	bOnly1St = true;
	ii++;
      } else if( argvstr == "-perCent" ) {
	percents.push_back(GmGenUtils::GetValue( argvstr1) );
	ii++;
      } else if( argvstr == "-distance" ) {
	distances.push_back(GmGenUtils::GetValue( argvstr1 ) );
	ii++;
      } else if( argvstr == "-perCent/distance" ) {
	thePercentDistances.push_back(std::pair<G4double,G4double>(GmGenUtils::GetValue( argvstr1 ),GmGenUtils::GetValue( argv[ii+2] ) ) );
      } else if( argvstr == "-bOutputFile" ) {
	theParamMgr->AddParam( argvName + " " + argvstr1, PTdouble );
	ii++;
      } else if( argvstr == "-norm" ) {
	if( G4String(argvstr1) == "atMaxima" ) {
	  theParamMgr->AddParam( argvName + " " + argvstr1, PTdouble );
	  ii++;
	} else if( G4String(argvstr1) == "atMaximum1" ) {
	  theNormType = GIDN_atMaximum1;
	  ii++;
	} else if( G4String(argvstr1) == "atPoint" ) {
	  theNormType = GIDN_atPoint;
	  if( argc < ii+4 ) {
	    G4Exception("getGammaIndex",
			"",
			FatalErrorInArgument,
			"WRONG NUMBER OF ARGUMENTS, THERE MUST BE AT LEAST 3 AFTER -norm atPoint");
	  }
	  thePointNorm = G4ThreeVector(GmGenUtils::GetValue(argv[ii+2]),GmGenUtils::GetValue(argv[ii+3]),GmGenUtils::GetValue(argv[ii+4]));
	  ii += 4;
	}
      } else if( argvstr == "-perCentType" ) {
	if( G4String(argvstr1) == "voxel" ) {
	  thePerCentType = GIPCT_Voxel;
	  ii++;
	} else if( G4String(argvstr1) == "maxImage" ) {
	  thePerCentType = GIPCT_ImageMax;
	  ii++;
	} else if( G4String(argvstr1) == "max2Images" ) {
	  thePerCentType = GIPCT_Max2Images;
	  ii++;
	} else if( G4String(argvstr1) == "atPoint" ) {
	  if( argc < ii+4 ) {
	    G4Exception("getGammaIndex",
			"",
			FatalErrorInArgument,
			"WRONG NUMBER OF ARGUMENTS, THERE MUST BE AT LEAST 3 AFTER -perCentType atPoint");
	  }
	  thePerCentType = GIPCT_AtPoint;
	  thePointPerCent = G4ThreeVector(GmGenUtils::GetValue(argv[ii+2]),GmGenUtils::GetValue(argv[ii+3]),GmGenUtils::GetValue(argv[ii+4]));
	  ii += 4;
	} else if( G4String(argvstr1) == "gammaOverCut" ) {
	  theGammaOverCut = GmGenUtils::GetValue( argvstr1 );
	  ii++;
	} else {
	  G4Exception("getGammaIndex",
		      "",
		      FatalErrorInArgument,
		      (G4String("WRONG ARGUMENT AFTER -relativeValue, IT MAY BE 1voxel, imageMax, WHILE IT IS ")+argvstr1).c_str());
	  
	}
	/*      } else if( argvstr == "-maxAtPoint" ) {
	if( argc < ii+3 ) {
	  G4Exception(theExeName,
		"Error",
		FatalErrorInArgument,
		"WRONG NUMBER OF ARGUMENTS, THERE MUST BE AT LEAST 3 AFTER -bMaxAtPoint");
	}
	bMaxAtPoint = true;
	theMaxAtPoint = G4ThreeVector(GmGenUtils::GetValue(argvstr1),GmGenUtils::GetValue(argv[ii+2]),GmGenUtils::GetValue(argv[ii+3]));
	ii += 3;*/
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

  if( thePerCentType == GIPCT_None ) {
    thePerCentType = GIPCT_Max2Images;
    if ( theNormType == GIDN_atPoint ) {
      thePerCentType = GIPCT_AtPoint;
      thePointPerCent = thePointNorm;
    }
  }

  bDrawXY = G4bool(theParamMgr->GetNumericValue("bDrawXY",1));
  bDrawXZ = G4bool(theParamMgr->GetNumericValue("bDrawXZ",0));
  bDrawYZ = G4bool(theParamMgr->GetNumericValue("bDrawYZ",0));
  bOutputFile = G4bool(theParamMgr->GetNumericValue("bOutputFile",1));
  bDistanceByVoxel = G4bool(theParamMgr->GetNumericValue("bDistanceByVoxel",0));
  //  theMaxAtPointFactor = theParamMgr->GetNumericValue("maxAtPointFactor",1);
  bPerSt = G4bool(theParamMgr->GetNumericValue("bPerSt",bPerSt));
  bGetGammaIndex = G4bool(theParamMgr->GetNumericValue("bGetGammaIndex",bGetGammaIndex));
  //  bDiffPerSt = G4bool(theParamMgr->GetNumericValue("bDiffPerSt",bDiffPerSt));
  theOnly1StName = theParamMgr->GetStringValue("only1St","");
  G4int palette = G4int(theParamMgr->GetNumericValue("palette",55));
  gStyle->SetPalette(palette);
  bUseMateID = G4bool(theParamMgr->GetNumericValue("bUseMateID",bUseMateID));
  bUseStructID = G4bool(theParamMgr->GetNumericValue("bUseStructID",bUseStructID));
  if( bUseMateID && bUseStructID ) {
    G4Exception(theExeName,
		"",
		FatalException,
		"You cannot select -bUseMateID and bUseStructID");
  }

  if( thePercentDistances.size() == 0 ) {
    //--- set percent and distance to default
    if( percents.size() == 0 ) {
      G4Exception(theExeName,
		  "",
		  JustWarning,
		  ("No percent set, setting it to "+GmGenUtils::ftoa(theDefaultPercent)).c_str());
      percents.push_back(theDefaultPercent);    
    }
    if( distances.size() == 0 ) {
      G4Exception(theExeName,
		  "",
		  JustWarning,
		  ("No distance set, setting it to "+GmGenUtils::ftoa(theDefaultDistance)).c_str());
      distances.push_back(theDefaultDistance);    
    }
    for( size_t ipc = 0; ipc < percents.size(); ipc++ ) {
      for(size_t id = 0; id < distances.size(); id++ )  {
	//	G4cout << thePercentDistances.size() << " adding PC " << ipc << " = " << percents[ipc] << " distance " << id << " = " << distances[id] << G4endl; //GDEB
	thePercentDistances.push_back(std::pair<G4double,G4double>(percents[ipc], distances[id]));
      }
    }
  }
   

}

//---------------------------------------------------------------------------
void DCMGetGammaIndex::CheckArguments()
{
  //----- CHECK CORRECT ARGUMENTS
  //--- THERE MUST BE AT LEAST TWO IMAGE FILES
  size_t nfct = theParamMgr->GetNofImageFiles("ANY");
  if( nfct < 2 ) {
    G4Exception(theExeName,
		"",
		FatalException,
		"THERE MUST BE AT LEAST TWO IMAGE FILES");
  }

  /*  if( bPerSt || bDiffPerSt ) {
    std::vector<DicomVImage*> images = theDicomMgr->GetImages();
    G4bool bIsOK = false;
    for( size_t ii = 0; ii < images.size(); ii++ ) {
      DIMModalityShort modS = images[ii]->GetModalityShort();
      if( modS == DIMS_STRUCT ) {
	bIsOK = true;
	break;
      }
    }
    if( !bIsOK ) G4Exception("getGammaIndex",
			     "",
			     FatalException,
			     "No file of type Structure");
			     }*/
	
}

//---------------------------------------------------------------------------
void DCMGetGammaIndex::ReadFilesAndGetImages()
{
  G4bool bMaxGammaValue = false;
  G4double theMaxGammaValue = DBL_MAX; 
  theMaxGammaValue = theParamMgr->GetNumericValue("maxGammaValue",theMaxGammaValue);
  if( theMaxGammaValue != DBL_MAX ) bMaxGammaValue = 1;

  theReaderMgr->ProcessData();

  //--- Check that there is an structure file it output by structure is requested
  if( bPerSt || bDiffPerSt ) {
    if( theReaderMgr->GetNofReaders(DRM_RTStruct) >= 1 ) {
      //      theStructImage = theDicomMgr->GetImages(DIM_RTStruct)[0];
    } else if( theReaderMgr->GetNofImageReaders(DRM_G4dcmCT) >= 1
	       && theDicomMgr->GetImages(DIM_G4dcmCT_StructID).size() >= 1 ) {
      //      theStructImage = theDicomMgr->GetImages(DIM_G4dcmCT_StructID)[0];
    } else {
      G4cout << " NIMAGES " << " " << theReaderMgr->GetNofImageReaders(DRM_G4dcmCT) << " " << theDicomMgr->GetNofImages(DIM_RTStruct) << G4endl; //GDEB
      G4Exception(theExeName,
		  "",
		  FatalException,
		  "aNo RTStruct image, while output per structure has been requested");
    }
  }

  //----- Initialize ROOT Canvas for GammaIndex histograms
  /*  TCanvas* canvas = new TCanvas;
  // canvas->SetCanvasSize(theNCanvasPixels,theNCanvasPixels);
  G4int theNCanvasPixels = 1000; //t
  canvas->SetCanvasSize(theNCanvasPixels,theNCanvasPixels*(fMaxY1Y2-fMinY1Y2)/(fMaxX1X2-fMinX1X2));
  canvas->Draw();
  canvas->cd();
  TPad* pad1 = 0;
  pad1 = new TPad("","",0.,0.,1.,1.);
  pad1->Draw("sames");
  pad1->cd(); 
  pad1->SetRightMargin(0.15); 
  gStyle->SetOptStat(0);
  std::map<G4double,TH2F*> theHistosXSvsGI;
  */

  DCM2DOperGammaIndex* operGammaIndex = new DCM2DOperGammaIndex();
  //  operGammaIndex->IsMaxAtPoint(bMaxAtPoint);
  //  operGammaIndex->SetMaxAtPoint(theMaxAtPoint);
  //  operGammaIndex->SetMaxAtPointFactor(theMaxAtPointFactor);
  //  operGammaIndex->SetMaxAtPointFactor(theMaxAtPointFactor);
  operGammaIndex->SetPerCentType(thePerCentType);
  operGammaIndex->SetPointPerCent(thePointPerCent);
  operGammaIndex->SetOnlyDiff(theParamMgr->GetNumericValue("bOnlyDiff",0));
  operGammaIndex->SetNErrorSigmas(theParamMgr->GetNumericValue("nErrorSigmas",0));   
  operGammaIndex->SetGammaOverCut(theGammaOverCut);

  if( theParamMgr->GetNumericValue("nErrorSigmas",0)!= 0 ) {
    bErrors = true;
  } else {
    bErrors = false;
  }
  /*
  //--- classify images by modality 
  std::set<DIMModalityShort>::const_iterator item;
  std::vector<std::pair<G4double,G4double> >::const_iterator itepdd;
  typedef std::multimap<DIModalityShort,DicomVImage*> mmsdi;
  mmsdi theImages2Compare;
  std::vector<DicomVImage*> images = theDicomMgr->GetImages();
  for( size_t ii = 0; ii < images.size(); ii++ ) {
    DIMModalityShort modS = images[ii]->GetModalityShort();
    if( theImages2Compare.find(modS) != theImages2Compare.end() ) theModalities.insert(modS); // at least a pair of images of the same modality
    theImages2Compare.insert(mmsdi::value_type(modS,images[ii]));
  }
  if( !theModalities.size() != 0 ) {
    G4Exception(theExeName,
		"",
		FatalException,
		"At least 2 files are needed of the same modality");
  }

  //-------- bPerSt: at least one RTStruct or G4DCMSt image
  if( bPerSt && theImages2Compare.find(DIMS_STRUCT) != theImages2Compare.end() ) {
    G4Exception(theExeName,
		"",
		FatalException,
		"bPerSt parameter is set to 1, but there is no STRUCTURE file (RTSTRUCT DICOM or G4DCM with structures");
  }

  //----- Loops to file pairs and compare pairs
  for( item = theModalities.begin(); item != theModalities.end(); item++ ) {
    DIMModalityShort modS = *item;   
    std::pair<mmsdi::const_iterator,mmsdi::const_iterator> pitei = theImages2Compare.equal_range(modS);
    mmsdi::const_iterator itei1,itei2;
    for( itei1 = pitei.first; itei1 != pitei.second; ite1++ ) { 
      for( itei2 = pitei.first; itei2 != pitei.second; ite2++ ) {
	if( itei1 == itei2 ) continue;
  */
  //--- By default exclude DIM_G4dcmCT_MateID, DIM_G4dcmCT_StructID, DIM_Sqdose_sq, DIM_Sqdose_err,
  std::vector<DicomVImage*> imagesAll = theReaderMgr->GetImages(true);
  std::vector<DicomVImage*> images;
    
  for( size_t ii = 0; ii < imagesAll.size(); ii++ ) {
    DicomVImage* img = imagesAll[ii];
    if(	img->GetModality() != DIM_Sqdose_sq &&
	img->GetModality() != DIM_Sqdose_err ) {
      if( bUseMateID ) {
	if( img->GetModality() != DIM_G4dcmCT_MateDens &&
	    img->GetModality() != DIM_G4dcmCT_StructID ) 
	  images.push_back(img);
      } else if ( bUseStructID ) {
	if( img->GetModality() != DIM_G4dcmCT_MateDens &&
	      img->GetModality() != DIM_G4dcmCT_MateID &&
	      img->GetModality() != DIM_G4dcmCT_StructID ) 
	  images.push_back(img);
      } else {
	//	G4cout << " IMAGE " <<  img->GetModalityStr() << G4endl; //GDEB
	if( img->GetModality() != DIM_G4dcmCT_MateDens &&
	    img->GetModality() != DIM_G4dcmCT_MateID &&
	    img->GetModality() != DIM_G4dcmCT_StructID ) 
	  images.push_back(img);
      }
    }
    G4cout << img->GetModalityStr() << " N IMAGES " << images.size() << G4endl; //GDEB
  }

  G4cout << " N IMAGES " << images.size() << G4endl; //GDEB
  DicomVDrawer* theDrawer = new DicomDrawerROOT();
  // get structure lines
  std::vector<DicomVReader*> VRTFiles = theReaderMgr->GetReaders(DRM_RTStruct, false);
  if( VRTFiles.size() > 1 ) {
    G4Exception("DCMGetGammaIndex::DrawImages",
		"",
		FatalException,
		"Only one RTStruct is allowed");
  } else if( VRTFiles.size() == 1 ) {
    DicomReaderRTStruct* RTFile = static_cast<DicomReaderRTStruct*>(VRTFiles[0]);
    DicomPolygonSet* theStPolygonSet = RTFile->GetPolygonSet();    
    theLineSets.push_back((DicomVLineSet*)theStPolygonSet);

    std::vector<DicomVLineList*> lineLists = theStPolygonSet->GetLineLists();
    /*    for( size_t ipl = 0; ipl < lineLists.size(); ipl++ ) {
      DicomVLineList* lineList = lineLists[ipl];
            G4cout << lineList << " BUILT LINELIST " << lineList->GetName() << " " << lineList->GetLines().size() << G4endl; //GDEB
	    } */
  }
  VRTFiles = theReaderMgr->GetReaders(DRM_TextLines, false);
  for( size_t ii = 0; ii < VRTFiles.size(); ii++ ) {
    DicomReaderTextLines* lineReader = dynamic_cast<DicomReaderTextLines*>(VRTFiles[ii]);
    theLineSets.push_back( lineReader->GetLineSet() );
  }
  
  if( bDiffPerSt ) {
    CalculateDiffPerSt();
  }
  
  if( !bGetGammaIndex ) return;

  if( DicomVerb(warningVerb) ) G4cout << " N images for GI " << images.size() <<" out of " << imagesAll.size() << G4endl;
  //  GmVVerbosity::SetVerbosityLevel( "DicomVerbosity", debugVerb );

  DicomVImage* imageErr1 = 0;;
  DicomVImage* imageErr2 = 0;;
  if( bErrors ) {
    std::vector<DicomVReader*> VSqFiles = theReaderMgr->GetReaders(DRM_Sqdose,false);
    if( VSqFiles.size() == 2 ) {
      DicomReaderSqdose* sqdoseReader1 = dynamic_cast<DicomReaderSqdose*>(VSqFiles[0]);
      imageErr1 = sqdoseReader1->GetDoseErrImage();
      DicomReaderSqdose* sqdoseReader2 = dynamic_cast<DicomReaderSqdose*>(VSqFiles[1]);
      imageErr2 = sqdoseReader2->GetDoseErrImage();
    }
  }

  //--- Modify image to count only image1Mod voxels in structure
  DicomVImageStr* theStructIDImage = 0;
  DicomReaderG4dcmCT* theReaderCT;
  if( bOnly1St ) {
    if( theReaderMgr->GetNofImageReaders(DRM_G4dcmCT) != 1
	   || theDicomMgr->GetImages(DIM_G4dcmCT_StructID).size() != 1 ) {
      G4Exception(theExeName.c_str(),
		  "",
		  FatalErrorInArgument,
		  "NO g4dcmCT file with structure ID");
    }
    //    DicomVReaderImage* readerImg = theReaderMgr->GetImageReaders(DRM_G4dcmCT)[0];
    theStructIDImage = static_cast<DicomVImageStr*>(theDicomMgr->GetImages(DIM_G4dcmCT_StructID)[0]);
    theReaderCT = static_cast<DicomReaderG4dcmCT*>(theReaderMgr->GetImageReaders(DRM_G4dcmCT)[0]);

  }

  G4String giImageName = "GammaIndex";
  for( size_t ii = 0; ii < images.size(); ii++ ) {
    for( size_t jj = ii+1; jj < images.size(); jj++ ) {
      if( images.size() > 2 )  giImageName = "GammaIndex:"+GmGenUtils::itoa(ii)+":"+GmGenUtils::itoa(jj);
      DicomVImage* image1 = images[ii];
      DicomVImage* image2 = images[jj];
      // if images are modified (e.g. rescaled) make a copy of them
      DicomVImage* image1Mod = image1;
      DicomVImage* image2Mod = image2;
      //?      DicomVImage* imageErr1Mod = imageErr1;
      //? DicomVImage* imageErr2Mod = imageErr2;
      DicomVImage* imageErr1Mod = 0;
      DicomVImage* imageErr2Mod = 0;
      //--- Normalize images
      //      G4cout << " NORMalize images " << theNormType << G4endl; //GDEB
      if( theNormType == GIDN_atMaxima ) {
	G4double maxVal1 = image1->GetMaxValue();
	G4double maxVal2 = image2->GetMaxValue();
	G4double maxVal12 = maxVal1/maxVal2;
	*image1Mod = *image1;
	image2Mod = new DicomVImage(*image2);
	DicomOperMultByNumber* multPer = new DicomOperMultByNumber(maxVal12);
	multPer->SetAutomaticOperate(false);
	multPer->Operate(image2Mod);
      } else if ( theNormType == GIDN_atMaximum1 ) {
	std::pair<G4double,G4ThreeVector> maxValPt1 = image1->GetMaxValueAndPoint();
	G4int ptExtN = G4int(theParamMgr->GetNumericValue("pointExtensionN",0));
	G4ThreeVector pointExtension(ptExtN*image1->GetVoxelDimX(),ptExtN*image1->GetVoxelDimY(),ptExtN*image1->GetVoxelDimZ());
	G4double maxVal2 = image2->GetValueAtPoint(maxValPt1.second,pointExtension);
	G4double maxVal12 = maxValPt1.first/maxVal2;
	image1Mod = new DicomVImage(*image1);
	image2Mod = new DicomVImage(*image2);
	DicomOperMultByNumber* multPer = new DicomOperMultByNumber(maxVal12);
	multPer->SetAutomaticOperate(false);
	multPer->Operate(image2Mod);
      } else if( theNormType == GIDN_atPoint ) {
	G4int ptExtN = G4int(theParamMgr->GetNumericValue("pointExtensionN",0));
	G4ThreeVector pointExtension(ptExtN*image1->GetVoxelDimX(),ptExtN*image1->GetVoxelDimY(),ptExtN*image1->GetVoxelDimZ());
	G4double maxVal1 = image1->GetValueAtPoint(thePointNorm,pointExtension);
	G4double maxVal2 = image2->GetValueAtPoint(thePointNorm,pointExtension);
	G4double maxVal12 = maxVal1/maxVal2;
	image1Mod = new DicomVImage(*image1);
	image2Mod = new DicomVImage(*image2);
	DicomOperMultByNumber* multPer = new DicomOperMultByNumber(maxVal12);
	multPer->SetAutomaticOperate(false);
	multPer->Operate(image2Mod);
	if( DicomVerb(debugVerb) ){
	  G4cout << thePointNorm << " image2 NORM_AT POINT " << maxVal12 << " = " << maxVal1 << " / " << maxVal2 << G4endl;
	  G4cout << " NEWMAX " << image2->GetValueAtPoint(thePointNorm,pointExtension) << G4endl; //GDEB
	}
	if( bErrors ) {
	  imageErr1Mod = new DicomVImage(*imageErr1);
	  imageErr2Mod = new DicomVImage(*imageErr2);
	  multPer->Operate(imageErr2Mod);
	}
      }
 
      //--- Modify image to count only image1Mod voxels in structure
      if( bOnly1St ) {
	std::map<G4int,G4String> stNames = theReaderCT->GetStructNames();
	G4int theOnly1StID = -1;
	for( std::map<G4int,G4String>::const_iterator itesn = stNames.begin(); itesn != stNames.end(); itesn++ ) {
	  //      G4cout << " theOnly1StID Names " << theOnly1StName << " " << itesn->second << " : " << GmGenUtils::AreWordsEquivalent(theOnly1StName,itesn->second) << " =? " << GmGenUtils::AreWordsEquivalent(itesn->second,theOnly1StName) << G4endl; //GDEB
	  G4String stName = itesn->second;
	  //	  G4cout << " STNAME " << theOnly1StName << " =?= " << stName << G4endl;//GDEB
	  if( stName.substr(stName.length()-2,2) == "_2" ) continue; // some patients have repeated structure: "GTV_50%EX" "GTV_50%EX_2"
	  if( GmGenUtils::AreWordsEquivalent(theOnly1StName,itesn->second) ) {
	    if( theOnly1StID != -1 ) {
	      G4Exception(theExeName.c_str(),
			  "",
			  FatalErrorInArgument,
			  ("TWO only1St structures found: "+theOnly1StName+" : "+GmGenUtils::itoa(theOnly1StID)+" "+GmGenUtils::itoa(itesn->first)).c_str());
	    }
	    theOnly1StID = itesn->first;
	    if( DicomVerb(debugVerb) )  G4cout << theOnly1StID << " STNAME  FOUND " << theOnly1StName << " =?= " << stName << G4endl;
	  }
	}
	//loop to voxels and make them 0 if not in structure
	G4int nVox = image1Mod->GetNoVoxels();
	//t check and correct if image1Mod and theStructIDImage are not the same
	if( nVox != G4int(theStructIDImage->GetNoVoxels()) ) {
	  G4Exception(theExeName.c_str(),
		      "",
		      FatalErrorInArgument,
		      "FIRST sqdose and g4dcm files have different number of voxels");
	}
    
      	std::vector<G4double>* image1ModData = image1Mod->GetData();
	G4int nVoxSt0 = 0;
	for( G4int iid = 0; iid < nVox; iid++ ) {
	  std::set<G4int> ists = theStructIDImage->GetIDList( iid );
	  G4bool bStFound = false;
	  for( std::set<G4int>::const_iterator itest = ists.begin(); itest != ists.end(); itest++ ) {
	    size_t ist = *itest;
	    //	if( iid % 100000 == 0 ) G4cout << iid << " Only1St " << ist << " ID " << theOnly1StID << " " << theOnly1StName << G4endl; //GDEB
	    if( G4int(ist) == theOnly1StID ) {
	      bStFound = true;
	      nVoxSt0 ++;
	      break;
	    }
	  }
	  if( !bStFound ) {
	    image1ModData->at(iid) = 0.;
	  }
	}
	if( DicomVerb(warningVerb) ) G4cout << "GetGammaIndex Only1St NVoxNon0 " << nVoxSt0 << " " << image1Mod->GetName() << G4endl;
      }
  
      std::vector< std::pair<G4double,G4double> >::const_iterator itepdd;
      for( itepdd = thePercentDistances.begin(); itepdd != thePercentDistances.end(); itepdd++ ) { // loop to pairs percent - distance
	if( ii == 0 ) G4cout << "GetGammaIndex: PERCENT= " << (*itepdd).first << " DISTANCE= " << (*itepdd).second << G4endl;
	if( bDrawXY ) {
	  //	    GIResultData resData =
	  //-	  DicomVImage* giImage = new DicomVImage(image1,"GammaIndex_XY_"+image1->GetModalityShortStr().substr(5,999),DIM_GammaIndex);
	  DicomVImage* giImage = new DicomVImage(image1,giImageName,DIM_GammaIndex);
	  //	  G4cout << " operGammaIndex->OperateXY " << G4endl << *image1Mod << G4endl << *image2Mod << " -> " << *giImage << G4endl; //GDEB
	  //	    std::vector<G4double> iData2 = *(image2Mod->GetData());
	  //	    std::vector<G4double> iData2Mod = *(image2->GetData());
	  //-	  operGammaIndex->OperateXY( image1Mod, image2Mod, (*itepdd).first, (*itepdd).second, 1, giImage );
	  operGammaIndex->OperateXY( image1Mod, image2Mod, imageErr1Mod, imageErr2Mod, (*itepdd).first, (*itepdd).second, 1, giImage );
	  theDicomMgr->OperateAll(giImage);
	  if( bMaxGammaValue ) SetImageMaxValue(giImage, theMaxGammaValue);
	  G4String extraFileName = "_"+GmGenUtils::ftoa((*itepdd).first)+"_"+GmGenUtils::ftoa((*itepdd).second)+"_2D";  
	  if( bOutputFile ) theDrawer->SetOutputFile("getGammaIndex"+extraFileName+".XY.root");
	  theDrawer->DrawXY(giImage,theLineSets, extraFileName);
	  //	    operGammaIndex->OperateXY( image1Mod, image2Mod, (*itepdd).first, (*itepdd).second, 1., giImage);
	  //?? just calling OperateXY, even if imageOut is not touched!!	    theDrawer->DrawXY(giImage,theLineSets);
	  //  resData->Draw("GammaIndex_"+image1->GetModalityShort());
	  //	    PrintResultData(resData);
	}

	if( bDrawXZ ) {
	  DicomVImage* giImage = new DicomVImage(image1,giImageName,DIM_GammaIndex);
	  G4cout << " operGammaIndex->OperateXZ " << G4endl << *image1Mod << G4endl << *image2Mod << G4endl; //GDEB
	  operGammaIndex->OperateXZ( image1Mod, image2Mod, (*itepdd).first, (*itepdd).second, 1., giImage);
	  theDicomMgr->OperateAll(giImage);
	  if( bMaxGammaValue ) SetImageMaxValue(giImage, theMaxGammaValue);  
	  G4String extraFileName = "_"+GmGenUtils::ftoa((*itepdd).first)+"_"+GmGenUtils::ftoa((*itepdd).second)+"_2D"; 
	  if( bOutputFile ) theDrawer->SetOutputFile("getGammaIndex"+extraFileName+".XZ.root");
	  theDrawer->DrawXZ(giImage,theLineSets,extraFileName);
	}
	if( bDrawYZ ) {
	  DicomVImage* giImage = new DicomVImage(image1,giImageName,DIM_GammaIndex);
	  G4cout << " operGammaIndex->OperateYZ " << G4endl << *image1Mod << G4endl << *image2Mod << G4endl; //GDEB
	  operGammaIndex->OperateYZ( image1Mod, image2Mod, (*itepdd).first, (*itepdd).second, 1., giImage);
	  theDicomMgr->OperateAll(giImage);
	  if( bMaxGammaValue ) SetImageMaxValue(giImage, theMaxGammaValue);  
	  G4String extraFileName = "_"+GmGenUtils::ftoa((*itepdd).first)+"_"+GmGenUtils::ftoa((*itepdd).second)+"_2D";
	  if( bOutputFile ) theDrawer->SetOutputFile("getGammaIndex"+extraFileName+".YZ.root");
	  theDrawer->DrawYZ(giImage,theLineSets,extraFileName);
	}
      }
    }
  }

  //    GmVVerbosity::SetVerbosityLevel( "DicomVerbosity", infoVerb );

}


//-------------------------------------------------------------------
void DCMGetGammaIndex::CalculateDiffPerSt()
{
  std::ofstream fout(theParamMgr->GetStringValue("fOutDiffPerSt","diffPerSt.out"));
  //--- make pairs of sqdose - sqdose_err if bDiffPerSt
  typedef std::pair<DicomVImage*,DicomVImage*> pii;
  std::vector<pii> imageWithErrors;
  std::vector<DicomVReaderImage*> VRTFiles = theReaderMgr->GetImageReaders(DRM_Sqdose, false);
  for( size_t ii = 0; ii < VRTFiles.size(); ii++ ) {
    DicomVReaderImage* reader = VRTFiles[ii];
    if( reader->GetModality() == DRM_Sqdose ) {
      DicomReaderSqdose* readerSqdose = static_cast<DicomReaderSqdose*>(reader);
      imageWithErrors.push_back(pii(readerSqdose->GetDoseImage(), readerSqdose->GetDoseErrImage()) );
    } else if( reader->GetModality() == DRM_RTDose ) {
      //      DicomReaderRTDose* readerSqdose = static_cast<DicomReaderRTDose*>(reader);
      imageWithErrors.push_back(pii(reader->GetImage(), (DicomVImage*)(0)));
    } else if( reader->GetModality() == DRM_G4dcmDose ) {
      //      DicomReaderG4dcmDose* readerSqdose = static_cast<DicomReaderG4dcmDose*>(reader);
      imageWithErrors.push_back(pii(reader->GetImage(), (DicomVImage*)(0)));
    }
  }
  //--- Loop to these pairs
  DicomOperIntersectStruct* operInters = new DicomOperIntersectStruct;
  //  DicomDrawerROOT* theDrawerRoot = new DicomDrawerROOT;
  theDicomMgr->SetRotateXY180(true);
   
  for( size_t ii = 0; ii < imageWithErrors.size()-1; ii++ ) {
    pii imgWErr1 = imageWithErrors[ii];
    for( size_t jj = 1; jj < imageWithErrors.size(); jj++ ) {
      if( ii == jj ) continue;
      pii imgWErr2 = imageWithErrors[jj];
      //--- check images are equal
      imgWErr1.first->CheckImageHeadersAreEqual( imgWErr2.first);
      //--- build OperIntersStruct with any of the images
      for( size_t ips = 0; ips < theLineSets.size(); ips++ ) {
	DicomVLineSet* lineSet = theLineSets[ips];
	if( lineSet->GetName().substr(0,9) != "RTStruct_" ) continue;
	DicomPolygonSet* polySet = dynamic_cast<DicomPolygonSet*>(lineSet);
	std::vector<DicomVLineList*> polyLists = polySet->GetLineLists();
	for( size_t ipl = 0; ipl < polyLists.size(); ipl++ ) {
	  DicomPolygonList* polyList = dynamic_cast<DicomPolygonList*>(polyLists[ipl]);
	  if( polyList->GetName().find("GTV") == std::string::npos ) continue;
	  DicomVImage* structImage = new DicomVImage( imgWErr1.first, "StructVolume_"+polyList->GetName(), DIM_OTHER );
	  G4cout << polyList << " POLYLIST " << polyList->GetName() << " " << polyList->GetLines().size() << G4endl; //GDEB
	  if( polyList->GetLines().size() == 0 ) continue;
	  operInters->Operate( structImage, polyList );
	  double diffSt = 0.;
	  double dose1St = 0.;
	  double dose2St = 0.;
	  double diffErrSt = 0.;
	  double dose1ErrSt = 0.;
	  double dose2ErrSt = 0.;
	  double volSt = 0.;
	  std::vector<G4double> dataSt = *(structImage->GetData());
	  std::vector<G4double> data1 = *(imgWErr1.first->GetData());
	  std::vector<G4double> data2 = *(imgWErr2.first->GetData());
	  std::vector<G4double>* dataErr1 = 0;
	  std::vector<G4double>* dataErr2 = 0;
	  if( imgWErr1.second != 0 ) dataErr1 = imgWErr1.second->GetData();  
	  if( imgWErr2.second != 0 ) dataErr2 = imgWErr2.second->GetData();
	  for( size_t iv = 0; iv < data1.size(); iv++ ) {
	    volSt += dataSt[iv];
	    diffSt += fabs(data1[iv] - data2[iv] ) * dataSt[iv];
	    dose1St += data1[iv] * dataSt[iv];
	    dose2St += data2[iv] * dataSt[iv];
	    if(  dataSt[iv] != 0 ) G4cout << polyList->GetName() << " " << iv << " diff " <<  fabs(data1[iv] - data2[iv] ) * dataSt[iv] << " " <<  data1[iv] * dataSt[iv] << " " <<  data2[iv] * dataSt[iv] << " vol " << dataSt[iv] << G4endl; //GDEB
	    if( dataErr1 ) {
	      diffErrSt += std::pow(dataErr1->at(iv),2) * dataSt[iv];
	      dose1ErrSt += std::pow(dataErr1->at(iv),2) * dataSt[iv];
	    }
	    if( dataErr2 ) {
	      diffErrSt += std::pow(dataErr2->at(iv),2) * dataSt[iv];
	      dose2ErrSt += std::pow(dataErr2->at(iv),2) * dataSt[iv];
	    }
	  }
	  // print difference of this structure
	  diffErrSt = std::sqrt(diffErrSt);
	  std::cout << "DIFF= " << std::setw(15) << polyList->GetName() << " " << diffSt << " +- " << diffErrSt << " VOL= " << volSt << " DOSE1= " << dose1St << " +- " << dose1ErrSt << " DOSE2= " << dose2St << " +- " << dose2ErrSt << std::endl; 
	  //	  fout << std::setw(15) << polyList->GetName() << " " << diffSt << " +- " << diffErrSt << " VOL= " << volSt << " DOSE1= " << dose1St << " +- " << dose1ErrSt << " DOSE2= " << dose2St << " +- " << dose2ErrSt << std::endl; 
	  fout << std::setw(15) << polyList->GetName() << " " << diffSt/volSt << " +- " << diffErrSt/volSt << " DOSE1= " << dose1St/volSt << " +- " << dose1ErrSt/volSt << " DOSE2= " << dose2St/volSt << " +- " << dose2ErrSt/volSt << " VOL= " << volSt << std::endl; 
	  //	  theDrawerRoot->DrawXY( structImage, theLineSets );
	  FILE* foutSq = fopen(("sqdose.DiffPerSt."+polyList->GetName()).c_str(),"wb");
	  structImage->DumpToSqdoseFile(foutSq);
	  std::ofstream foutDcm(("g4dcm.DiffPerSt."+polyList->GetName()).c_str());
	  structImage->DumpHeaderToTextFile(foutDcm);
	  structImage->DumpDataToTextFile(foutDcm);	  
	  delete structImage;
	}
      }
    }
  }
  fout.close();
}

//-------------------------------------------------------------------
void DCMGetGammaIndex::SetImageMaxValue(DicomVImage* image, G4double maxValue)
{
  std::vector<G4double>* iData = image->GetData();
  size_t nVoxels = image->GetNoVoxels();
  for( size_t ii = 0; ii < nVoxels; ii++ ) {
    //    if(  iData->at(ii) != 0 ) G4cout << " SetImageMaxValue " << ii << " " << iData->at(ii) << G4endl; //GDEB
    if( iData->at(ii) > maxValue ) {
      iData->at(ii) = maxValue;
    }
  }

}

//-------------------------------------------------------------------
void DCMGetGammaIndex::PrintHelp()
{
 G4cout << "@@@ LIST OF ARGUMENTS: " << G4endl
	<< " -fSqDose       sqdose file name" << G4endl
	<< " -fDCMDose      DCMDose file name" << G4endl
        << " -percent       gamma index percentage " << G4endl
        << " -distance      gamma index distance " << G4endl
	<< " -fRTStruct     DCM file with the structures: gamma index per structure will be calculated" << G4endl
	<< " -norm          normalization of both images (by default no normalization) " << G4endl
	<< "         normalization types: atMaxima (maximum of the two images)/ atMaximum1 (maximum of first image) / atPoint POINT_X POINT_Y POINT_Z (value of the first image at point)" << G4endl
	<< " -perCentType   way to calculate the maximum with respect to which per cent values will be calculated " << G4endl
	<< "         percent types: max2Images (use the maximum of the first and second images; default), maxImage (use the maximum of the first image), voxel (for each voxel analysed use the value of it in the first image), atPoint POINT_X POINT_Y POINT_Z (use the value of the voxel at the first image where the point is) " << G4endl
   //	<< " -maxDistVoxel  maximum number of voxels that will be searched for building the gamma index" << G4endl
   //	<< " -maxAtPoint    dose of voxel at point is taken as maximum (bigger dose will be made equal to this * maxAtPointFactor)" << G4endl
   //	<< " -maxAtPointFactor  factor to define maximum voxel dose = dose_at_point*maxAtPointFactor (default is 1.)" << G4endl
	<< " -maxGammaValue  maximum value of gamma index" << G4endl
	<< " -bInterpolateVoxels  values of second image are obtained 0 (default): value of voxel in image2 corresponding to centre of voxel in image1 or 1: interpolating linearly voxel values " << G4endl
	<< " GIPCT_ImageMax, GIPCT_Voxel1st, GIPCT_AtPoint " << G4endl
	<< " -bDistanceByVoxel   calculate distance by distance between the closest voxel centre and other voxel centres, instead of distance between first image voxel centre and other voxel centres" << G4endl
	<< " -nErrorSigmas   Use error_sigma*nErrorSigmas to get closest gamma index percentage (positive difference between closest points of value +- error)" << G4endl
	<< " -bDrawXY 0/1   option to draw XY plane slices with structures superimposed (default = 1)" << G4endl
	<< " -bDrawXZ 0/1   option to draw XZ plane slices (default = 0)" << G4endl
	<< " -bDrawYZ 0/1   option to draw YZ plane slices (default = 0)" << G4endl
	<< " -palette NUMBER   choose integer code of color palette (for codes see https://root.cern.ch/doc/master/classTColor.html) " << G4endl
	<< " -paletteMin VALUE   set minimum of palette" << G4endl
	<< " -paletteMax VALUE   set maximum of palette" << G4endl
	<< " -nPixelsX VALUE   number of pixels used for each figure in the X axis. Default is 1000. " << G4endl
	<< " -nPixelsY VALUE   number of pixels used for each figure in the Y axis. Default is 1000. " << G4endl
	<< " -nPixelsZ VALUE   number of pixels used for each figure in the Z axis. Default is 1000. " << G4endl
	<< " -imageFormat FORMAT   available format are jpg (default), png, eps, ps, pdf, svg, xpm and tiff " << G4endl
	<< " -bOutputFile    write the 2D histograms in an output file getGammaIndex_PC_DIST_2D.root" << G4endl
	<< " -verb          verbosity: sets the verbosity" << G4endl
	<< " -help          prints the list of arguments " << G4endl;
}

  
