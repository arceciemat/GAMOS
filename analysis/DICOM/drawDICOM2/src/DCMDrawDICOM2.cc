#include "DCMDrawDICOM2.hh"

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
#include "DICOM/DICOMReaders/include/DicomReaderSqdose.hh"

#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"
#include "TStyle.h"

//---------------------------------------------------------------------------
DCMDrawDICOM2::DCMDrawDICOM2()
{
  theExeName = "drawDICOM2";
  theDicomMgr = DicomMgr::GetInstance();
  theReaderMgr = DicomReaderMgr::GetInstance();
}

//---------------------------------------------------------------------------
void DCMDrawDICOM2::ProcessArguments(int argc,char** argv)
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
		 || argvstr == "-bDrawIsodLines" 
		 || argvstr == "-overSurrounding" ) {
	theParamMgr->AddParam( argvName + " " + argvstr1, PTdouble );
	ii++;
      } else if( argvstr == "-colorFraction" 
		 || argvstr == "-imageMax1" 
		 || argvstr == "-imageMax2" ) {
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
  
  SetPalette();

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
    G4Exception("DCMDrawDICOM2::DCMDrawDICOM2",
		"",
		FatalErrorInArgument,
		"You cannot give a list of IsodPerCents and another of IsodPerCentsAbs, as only one would be used");
  }

  if( theIsodPerCents.size() + theIsodPerCentsAbs.size() != 0 ) {
    if( !bDrawIsodLines ) {
      G4Exception("DCMDrawDICOM2::DCMDrawDICOM2",
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

  //------ Get first and second images
  for( G4int ii = 1; ii < argc; ii+=2 ){
    G4String argvstr = argv[ii];
    if( argvstr == "-fG4dcmCT" ||
	argvstr == "-fG4dcmNM" ||
	argvstr == "-fG4dcmDose" ||
	argvstr == "-fSqdose" ||
	argvstr == "-fCT" ||
	argvstr == "-fNM" ||
	argvstr == "-fRTDose" ||
	argvstr == "-fInterfile" ) {
      theFileNames.push_back( argv[ii+1] );
      G4cout << " theFileNames " << argv[ii+1] << G4endl; //GDEB
    }
  } 

}

//---------------------------------------------------------------------------
void DCMDrawDICOM2::SetPalette()
{

  G4int palette = G4int(theParamMgr->GetNumericValue("palette",55));
  gStyle->SetPalette(palette);

  if( palette < 100 ) {
    return;
  } else {
    if( palette == 101 ) {
      UInt_t Number = 3;
      Double_t Red[3] = { 0.00, 0.50, 1.00};
      Double_t Green[3]  = { 0.00, 0.00, 0.00};
      Double_t Blue[3]   = { 0.00, 0.00, 0.00};
      Double_t Length[3] = { 0.00, 0.50, 1.00 };
      Int_t nb=50;
      //   static Int_t  CreateGradientColorTable(UInt_t Number, Double_t* Stops, Double_t* Red, Double_t* Green, Double_t* Blue, UInt_t NColors, Float_t alpha=1.);
      TColor::CreateGradientColorTable(Number,Length,Red,Green,Blue,nb);
    } else if( palette == 102 ) {
      UInt_t Number = 3;
      Double_t Red[3] = { 0.00, 0.00, 0.00};
      Double_t Green[3]  = { 0.00, 0.50, 1.00};
      Double_t Blue[3]   = { 0.00, 0.00, 0.00};
      Double_t Length[3] = { 0.00, 0.50, 1.00 };
      Int_t nb=50;
      //   static Int_t  CreateGradientColorTable(UInt_t Number, Double_t* Stops, Double_t* Red, Double_t* Green, Double_t* Blue, UInt_t NColors, Float_t alpha=1.);
      TColor::CreateGradientColorTable(Number,Length,Red,Green,Blue,nb);
    } else if( palette == 103 ) {
      UInt_t Number = 3;
      Double_t Red[3] = { 0.00, 0.00, 0.00};
      Double_t Green[3]  = { 0.00, 0.00, 0.00};
      Double_t Blue[3]   = { 0.00, 0.50, 1.00};
      Double_t Length[3] = { 0.00, 0.50, 1.00 };
      Int_t nb=50;
      //   static Int_t  CreateGradientColorTable(UInt_t Number, Double_t* Stops, Double_t* Red, Double_t* Green, Double_t* Blue, UInt_t NColors, Float_t alpha=1.);
      TColor::CreateGradientColorTable(Number,Length,Red,Green,Blue,nb);
    }
  }
  /*  Int_t myPalette[5];
  myPalette[0] = 1;
  myPalette[1] = 2;
  myPalette[2] = 4;
  myPalette[3] = 6;
  myPalette[4] = 8;
   gStyle->SetPalette(5,myPalette);
  */
}
  
//---------------------------------------------------------------------------
void DCMDrawDICOM2::CheckArguments()
{
  //----- CHECK CORRECT ARGUMENTS
  size_t nfct = theParamMgr->GetNofImageFiles("ANY");
  if( nfct != 2 ) {
    DicomException("THERE MUST BE TWO AND ONLY TWO IMAGE FILES");
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
  
  if( bDrawMateID && bDrawStructID ) { 
    DicomException("bDrawMateID & bDrawStructID CANNOT BE BOTH CHOSEN");
  }

}

//---------------------------------------------------------------------------
void DCMDrawDICOM2::ReadFilesAndGetImages()
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
void DCMDrawDICOM2::DrawImages()
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
    G4Exception("DCMDrawDICOM2::DrawImages",
		"",
		FatalException,
		"Only one RTStruct is allowed");
  } else if( VRTFiles.size() == 1 ) {
    RTFile = dynamic_cast<DicomReaderRTStruct*>(VRTFiles[0]);
    theStPolygonSet = RTFile->GetPolygonSet();
    theVLineSets.push_back(theStPolygonSet);
  }

  mmmdri imageReaders = theReaderMgr->GetImageReaders();
  std::vector<DicomVImage*> images; 
  for( size_t ii = 0; ii < 2; ii++ ) {
    for( mmmdri::const_iterator ite = imageReaders.begin(); ite != imageReaders.end(); ite++ ){
      G4cout << " TEST NAME " << ite->second->GetFileName() << "==" << theFileNames[ii] << G4endl; //GDEB
      if( ite->second->GetFileName() == theFileNames[ii] ) {
	DicomVImage* image = GetImage( ite->second );
	images.push_back( image );
	if( DicomVerb(infoVerb) ) G4cout << " DCMDrawDICOM2  IMAGE FOUND " << theFileNames[ii] << " " << image->GetModalityStr() << G4endl; 
      }
    }
  }

  //----- BUILD NEW IMAGE 
  DicomVImage* combImage = BuildCombinedImage( images[0], images[1] );
  DrawImage(combImage,theVLineSets);

}

//-------------------------------------------------------------------
DicomVImage* DCMDrawDICOM2::GetImage( DicomVReaderImage* imageReader ) 
{
 if( imageReader->GetModality() == DRM_G4dcmCT ) {
   DicomReaderG4dcmCT* readerCT = dynamic_cast<DicomReaderG4dcmCT*>(imageReader);
  //---- Replace MateDens by MateID or StructID if requested
   if( bDrawMateID ) {
     return readerCT->GetMateIDImage();
   } else if( bDrawStructID ) {
     return readerCT->GetStructIDImage();
   } else {
     return readerCT->GetMateDensImage();
   }
 } else if( imageReader->GetModality() == DRM_Sqdose ) {
   //---- Replace Sqdose by doseSq, doseErr, doseErrRel
   G4String doseVar = theParamMgr->GetStringValue("doseVar","");
   if( doseVar == "" || doseVar == "dose" ) {
     return imageReader->GetImage();
   } else {
     DicomReaderSqdose* readerSq = dynamic_cast<DicomReaderSqdose*>(imageReader);
     //    G4cout << " DOSEVAR " << doseVar << G4endl; //GDEB
     if( doseVar == "doseSq" ) {
       return readerSq->GetDoseSqImage();
     } else if( doseVar == "doseErr" ) {
       return readerSq->GetDoseErrImage();
     } else if( doseVar == "doseErrRel" ) {
       return readerSq->CreateImage("Sqdose_errRel", DIM_Sqdose_errRel, false, true );
     } else {
       G4Exception("DCMDrawDICOM2::DrawImages",
		   "",
		  FatalException,
		  ("Argument after -doseVar  must be dose/doseSq/doseErr/doseErrRel , while it is " + doseVar).c_str());
     }
   }
 } else {
   return imageReader->GetImage();
 }

}

//-------------------------------------------------------------------
DicomVImage* DCMDrawDICOM2::BuildCombinedImage( DicomVImage* image1, DicomVImage* image2 )
{
  std::vector<G4double>* imgDatap1 = image1->GetData();
  G4double* imgData1 = &(imgDatap1->at(0));
  std::vector<G4double>* imgDatap2 = image2->GetData();
  G4double* imgData2 = &(imgDatap2->at(0));
  double colorFraction = theParamMgr->GetNumericValue("colorFraction",0.5);
  // TEST BOTH IMAGES HAVE SAME SIZE
  if( image1->GetNoVoxelsX() != image2->GetNoVoxelsX() || 
      image1->GetNoVoxelsY() != image2->GetNoVoxelsY() || 
      image1->GetNoVoxelsZ() != image2->GetNoVoxelsZ() ) {
    G4cerr << " IMAGES DIMENSIONS: X= " << image1->GetNoVoxelsX() << " " << image2->GetNoVoxelsX() 
	   << "                    Y= " << image1->GetNoVoxelsY() << " " << image2->GetNoVoxelsY() 
	   << "                    Z= " << image1->GetNoVoxelsZ() << " " << image2->GetNoVoxelsZ() << G4endl;
    DicomException(" IMAGES HAVE DIFFERENT DIMENSIONS");
  }

  size_t nVoxels = image1->GetNoVoxels();
  G4String imageName = "ImageCombined."+image1->GetModalityStr().substr(4,99)+"."+image2->GetModalityStr().substr(4,99);
  DicomVImage* combImage = new DicomVImage(image1,imageName,DIM_OTHER);
  std::vector<G4double>* imgCombData = new std::vector<G4double>(nVoxels);
  combImage->SetData( imgCombData );
  G4double max1 = image1->GetMaxValue();
  max1 = std::min(max1,theParamMgr->GetNumericValue("imageMax1",DBL_MAX));
  G4double max2 = image2->GetMaxValue();
  max2 = std::min(max2,theParamMgr->GetNumericValue("imageMax2",DBL_MAX));
  G4cout << " DCMDrawDICOM2 max's " << max1 << " " << max2 << G4endl; //GDEB
  for( size_t ii = 0; ii < nVoxels; ii++, imgData1++, imgData2++ ) {
    G4double value = (*imgData1)/max1*colorFraction+(*imgData2)/max2*(1-colorFraction);
    imgCombData->at(ii) = value;
  }

  return combImage;
}

//-------------------------------------------------------------------
void DCMDrawDICOM2::DrawImage(DicomVImage* image, std::vector<DicomVLineSet*> lineSets, G4String extraFileName)
{
  //  G4cout << " DrawImage DrawXY " << bDrawXY << G4endl; //GDEB
  std::vector<DicomVReader*> VRTFiles = theReaderMgr->GetReaders(DRM_TextLines, false);
  for( size_t ii = 0; ii < VRTFiles.size(); ii++ ) {
    DicomReaderTextLines* lineReader = dynamic_cast<DicomReaderTextLines*>(VRTFiles[ii]);
    //    G4cout << " DCMDrawDICOM2::DrawImage( GetReaders(DRM_TextLines lineReader "<< lineReader << G4endl; //GDEB
    lineSets.push_back( lineReader->GetLineSet() );
  }
  //@@@ build isodose values
  std::vector<G4double> isodoseValues;
  if( theIsodPerCents.size() != 0 ) {
    G4double maxValue = image->GetMaxValue();
    for( size_t iisod = 0; iisod < theIsodPerCents.size(); iisod++ ) {
      isodoseValues.push_back(theIsodPerCents[iisod]/100.*maxValue);
      if( DicomVerb(debugVerb) ) G4cout << " DCMDrawDICOM2::DrawImage isodose " << iisod << " = " << isodoseValues[iisod]<< " = " << theIsodPerCents[iisod] << " /100* " << maxValue << std::endl;
    }
  }
  if( theIsodPerCentsAbs.size() != 0 ) {
    for( size_t iisod = 0; iisod < theIsodPerCentsAbs.size(); iisod++ ) {
      isodoseValues.push_back(theIsodPerCentsAbs[iisod]);
      if( DicomVerb(debugVerb) ) G4cout << " DCMDrawDICOM2::DrawImage isodose " << iisod << " = " << isodoseValues[iisod] << std::endl;
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
void DCMDrawDICOM2::PrintHelp()
{
  ((DicomParameterMgr*)(DicomParameterMgr::GetInstance()))->PrintHelpInputFiles();
  G4cout << " -colorFraction        Fraction of first image (second image 1-F)" 
	 << " -imageMax1            Maximum value to normalize fisrt figure" 
	 << " -imageMax2            Maximum value to normalize fisrt figure" 
	 << " -readerType READER_TYPE   type of reader (default = ROOT) " << G4endl
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
	 << " -imageFormat FORMAT   available format are gif (default), png, eps, ps, pdf, svg, xpm, jpg and tiff " << G4endl
	 << " -fDrawLines FILE_NAME   file where line format is defined (color, style, width, ...) " << G4endl
  	 << " -rotateXY DEGREES   figures are rotated by VAL in XY plane (=around Z axis)" << G4endl
	 << " -printMax  0/1   prints the maximum value of any voxel in each image together with the X/Y/Z voxel id's and X/Y/Z position of the voxel center" << G4endl
	 << " -printValueAtPoint POINT_X POINT_Y POINT_Z*: prints the value at the voxel in each image corresponding to the POINT together with the X/Y/Z voxel id's and X/Y/Z position of the voxel center" << G4endl
	 << " -bDrawIsodLines 1/0   draw isodose lines or not (default)" << G4endl  
	 << " -isodPerCents N_VALUES %VALUE_1 %VALUE_2 ...    Define the isodose percentages (w.r.t. to maximum value in image). Default is 90 50 10" << G4endl
	 << " -isodPerCentsAbs N_VALUES %VALUE_1 %VALUE_2 ...    Define the isodose percentages (in absolute values). Default is 90 50 10" << G4endl
	 << " -overSurrounding FACTOR   limit voxel values to FACTOR*average values of surrounding voxels" << G4endl;

}
  
