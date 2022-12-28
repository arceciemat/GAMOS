#include "DCMGetOrganInfo.hh"

#include "TStyle.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TLine.h"

#include "DICOM/DICOMBase/include/DicomVImage.hh"
#include "DICOM/DICOMBase/include/DicomMgr.hh"
#include "DICOM/DICOMBase/include/DicomParameterMgr.hh"
#include "DICOM/DICOMBase/include/DicomPolygonSet.hh"
#include "DICOM/DICOMBase/include/DicomVerbosity.hh"
#include "DICOM/DICOMReaders/include/DicomReaderG4dcmCT.hh"
#include "DICOM/DICOMReaders/include/DicomReaderSqdose.hh"
#include "DICOM/DICOMReaders/include/DicomReaderMgr.hh"
#include "DICOM/DICOMReaders/include/DicomReaderRTStruct.hh"
#include "DICOM/DICOMReaders/include/DicomReaderG4dcmNM.hh"
#include "DICOM/DICOMBase/include/Dicom2DOperAddError.hh"
#include "DICOM/DICOMBase/include/DicomNDOperArithm.hh"
#include "DICOM/DICOMBase/include/DicomDrawerROOT.hh"

#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"

#include "CLHEP/Random/RandGauss.h"
#include "ROOTUtilities/PlotData/Definitions.C"
#include "ROOTUtilities/PlotData/PlotUtils.C"
#include "ROOTUtilities/GenUtils.C"

using namespace std;

//---------------------------------------------------------------------------
DCMGetOrganInfo::DCMGetOrganInfo()
{
  Initialise();
  theParamMgr = (DicomParameterMgr*)(DicomParameterMgr::GetInstance());
  theReaderMgr = DicomReaderMgr::GetInstance();
  theDMgr = DicomMgr::GetInstance();

  thePerCentImage = (DicomVImage*)(0);
  theErrorImageRelErr = 0.;
  theAnalyseImageRandom = (DicomVImage*)0;
}

//---------------------------------------------------------------------------
DCMGetOrganInfo::~DCMGetOrganInfo()
{

  if(theAnalyseImage) delete theAnalyseImage;
  if(theAnalyseSqImage) delete theAnalyseSqImage;
  if(theAnalyseErrorImage) delete theAnalyseErrorImage;
  if(theStructIDImage) delete theStructIDImage;
  if(theAnalyseMateDensImage) delete theAnalyseMateDensImage;
  
}

//---------------------------------------------------------------------------
void DCMGetOrganInfo::Initialise()
{
  bDepoEnergy = false;
  bVolumeMass = false;
  bPerMate = false;
  
  theExeName = "getOrganInfo";
  
  theStLinesFN = "";
  theNColorsInFile = 0;

  theAnalyseImage = 0;
  theAnalyseErrorImage = 0;
  theAnalyseSqImage = 0;
  theStructIDImage = 0;
  theAnalyseMateDensImage = 0;

  bDosimQ = false;
  theNofEvents = 1;
  thePrescribedDose = -1;
  thePrintStID = -1;
  thePrintStName = "";
  theHistosLogN = 0.;
  bHistosLogN = false;
}

//---------------------------------------------------------------------------
void DCMGetOrganInfo::ProcessArguments(int argc,char** argv)
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
      DicomException(G4String("ONLY ONE ARGUMENT: ") + argv[1]);
    }
  } else {
    if(argc%2 != 1) { 
      G4Exception(theExeName.c_str(),
		  "Error",
		  FatalErrorInArgument,
		  "WRONG NUMBER OF ARGUMENTS: THEY MUST BE -XX1 VAL_XX1 -XX2 VAL_XX2 ... ");
    }
    
    for( G4int ii = 1; ii < argc; ii++ ){
      G4String argvstr = argv[ii];
      //      std::cout << ii << "ARGV " << argv[ii] << std::endl; //GDEB
      G4String argvstr1 = argv[ii+1];
      G4String argvName = argvstr.substr(1,999);
      if( argvstr == "-bDepoEnergy" ||
	  argvstr == "-bVolumeMass" ||
	  argvstr == "-bPerMate" ||
	  argvstr == "-bHistos" ||
	  //	  argvstr == "-multFactor" ||
	  argvstr == "-minHistoDose" ||
	  argvstr == "-maxHistoDose" ||
	  argvstr == "-bLogOrganInfo" || 
	  argvstr == "-prescribedDose" || 
	  argvstr == "-nRandomizeError" ||
	  argvstr == "-plusError" ||
	  argvstr == "-minusError" ||
	  argvstr == "-nErrorSigmas" ||
	  argvstr == "-errorImageRelErr" || 
	  argvstr == "-multDoseByNumber" ||
	  argvstr == "-palette" ||
	  argvstr == "-paletteMin" ||
	  argvstr == "-paletteMax" ||
	  argvstr == "-histosLogN" ||
	  argvstr == "-bDrawImageDoseInSt" ){
	theParamMgr->AddParam( argvName + " " + argvstr1, PTdouble );
	ii++;
      } else if( argvstr == "-dosimQuantity" ||
		 argvstr == "-dosimQ" ) {
	theDosimQuantities.push_back(argvstr1);
	bDosimQ = true;
	ii++;
      } else if( argvstr == "-printSt" ) {
	thePrintStName = argvstr1;
	ii++;
      } else if( argvstr == "-excludeStNames" ) {
	G4int nex = GmGenUtils::GetInt(argvstr1);
	for( G4int ie = 0; ie < nex; ie++ ) {
	  theExcludeStNames.push_back(argv[ii+2+ie]);
	}
	ii += nex+2;
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

  bDepoEnergy = G4bool(theParamMgr->GetNumericValue("bDepoEnergy",0));
  if( !bDepoEnergy ) {
    theOutputName = "getOrganDose";
  } else {
    theOutputName = "getOrganEDep";
  }
  bPerMate = G4bool(theParamMgr->GetNumericValue("bPerMate",0));
  bHistos = G4bool(theParamMgr->GetNumericValue("bHistos",1));
  bVolumeMass = G4bool(theParamMgr->GetNumericValue("bVolume",0));
  bLog = G4bool(theParamMgr->GetNumericValue("bLogOrganInfo",0));
  //  theMultFactor = theParamMgr->GetNumericValue("multFactor",1);
  theNofEvents = theParamMgr->GetNumericValue("multDoseByNumber",1);
  thePrescribedDose = theParamMgr->GetNumericValue("prescribedDose",thePrescribedDose);
  theHistoDoseMin = theParamMgr->GetNumericValue("minHistoDose",0.);
  theHistoDoseMax = theParamMgr->GetNumericValue("maxHistoDose",DBL_MAX);
  bUserHistoDoseMin = theParamMgr->IsParameterInScript("minHistoDose");
  bUserHistoDoseMax = theParamMgr->IsParameterInScript("maxHistoDose");
  
  if( bVolumeMass ) {  // SET BY USER 
    theAnalyseModality = DIM_G4dcmCT_MateDens;

  } else { // NO USER DEFINED SET IT EQUAL TO THE INPUT FILE TYPE 
    if(  theParamMgr->GetNofImageFiles("fRTDose") != 0 ) {
      theAnalyseModality = DIM_RTDose;
    } else if( theParamMgr->GetNofImageFiles("fSqdose") != 0 ) {
      theAnalyseModality = DIM_Sqdose;
      //      G4cout << " ANALYSEMODALITY SQ " << DicomVImge::GetModalityStr(theAnalyseModality) << G4endl; //GDEB
    } else if( theParamMgr->GetNofImageFiles("fG4dcmDose") != 0 ) {
      theAnalyseModality = DIM_G4dcmDose;
    } else if( theParamMgr->GetNofImageFiles("fNM") != 0 ) {
      theAnalyseModality = DIM_NM;
    } else if( theParamMgr->GetNofImageFiles("fG4dcmNM") != 0 ) {
      theAnalyseModality = DIM_G4dcmNM;
    }
  }
  G4int palette = G4int(theParamMgr->GetNumericValue("palette",55));
  gStyle->SetPalette(palette);
  bDrawImageDoseInSt = G4bool(theParamMgr->GetNumericValue("bDrawImageDoseInSt",0));
  theHistosLogN = theParamMgr->GetNumericValue("histosLogN",0.);
  if( theHistosLogN != 0. ) bHistosLogN = true;
  //  G4cout << " ANALYSEMODALITY " << DicomVImage::GetModalityStr(theAnalyseModality) << G4endl; //GDEB
  theNErrorSigmas = GmParameterMgr::GetInstance()->GetNumericValue("nErrorSigmas",1);
  theErrorImageRelErr = theParamMgr->GetNumericValue("errorImageRelErr",theErrorImageRelErr);
}

//---------------------------------------------------------------------------
void DCMGetOrganInfo::CheckArguments()
{
  //----- CHECK CORRECT ARGUMENTS
  //--- THERE MUST BE AN OUTPUT FILE
  if( theParamMgr->GetStringValue("fOut","") == "" ) {
    PrintHelp();
    G4Exception(theExeName.c_str(),
		"",
		FatalErrorInArgument,
		"NO VALUE GIVEN FOR output FILE");
  }

  size_t nfct = theParamMgr->GetNofImageFiles("fG4dcmCT");
  if( nfct != 1 ) {
    DicomException("THERE MUST BE ONE AND ONLY ONE G4dcmCT FILE ");
  }

  //  G4cout << " NIMAGES " << theParamMgr->GetNofImageFiles("ANY") << G4endl; //GDEB
  size_t nfnm = theParamMgr->GetNofImageFiles("fG4dcmNM");
  if( bVolumeMass ) {
    if( bDepoEnergy ) {
      DicomException("CANNOT ASK TO CALCULATE Deposited Energy IF VOLUME IS TO BE CALCULATED");
    }
    if( theParamMgr->GetNofImageFiles("ANY") - nfct - nfnm != 0 ) {
      DicomException("THERE CANNOT BE ANY DOSE OR NM FILE");
    }
  } else {
    if( theParamMgr->GetNofImageFiles("ANY") - nfct - nfnm != 1 ) {
      G4cerr << "NIMAGES " << theParamMgr->GetNofImageFiles("ANY") - nfct - nfnm << " : " << theParamMgr->GetNofImageFiles("ANY") << " - " << nfct << " - " << nfnm << G4endl; 
      DicomException("aTHERE CAN BE ONE AND ONLY ONE DOSE OR NM FILE TO EXTRACT DATA FROM");
    }
  }

  /*t  if( bPerMate ) {
    std::vector<G4String> files = theParamMgr->GetVStringValue("fStruct",files);
    if( files.size() != 1 ) {
      DicomException("DOSE/ACTIVITY IS TO BE CALCULATED BY MATERIAL INDEX, THEREFORE NO DICOM RTSTRUCT FILE IS REQUIRED");
    }
  }
  */
  
}

//---------------------------------------------------------------------------
void DCMGetOrganInfo::ReadFilesAndGetImages()
{
//--- READ AND BUILD IMAGES
  theReaderMgr->ProcessData();
 
  if( theDMgr->GetNofImages(theAnalyseModality) == 1 ) {
    //---- IMAGE TO BE ANALYSED (dose/NM/G4dcm_MateDens(to get volume))
    theAnalyseImage = theDMgr->GetImages(theAnalyseModality)[0];
    //    *theAnalyseImage *= theMultFactor;
    theAnalyseErrorImage = 0;
    if( theAnalyseModality == DIM_Sqdose ) {// CONTAINS ERRORS
      theAnalyseErrorImage = theDMgr->GetImages(DIM_Sqdose_err)[0];
      theAnalyseSqImage = theDMgr->GetImages(DIM_Sqdose_sq)[0];
      //      *theAnalyseErrorImage *= theMultFactor;
    }
    if( theErrorImageRelErr != 0 ) theAnalyseErrorImage = BuildAnalyseErrorImage( theAnalyseImage, theErrorImageRelErr );

    //---- IMAGE TO CALCULATE THE MASS (DENSITY)
    theAnalyseMateDensImage = theDMgr->GetImages(DIM_G4dcmCT_MateDens)[0];
    theAnalyseMateDensImage->PrintHeader(G4cout);
    if( !theAnalyseMateDensImage ) {
      PrintHelp();
      DicomException("NO IMAGE TO BE ANALYSED CONTAINING MATERIAL DENSITIES ");
    }
    
  } else {
    //    theParamMgr->PrintParametersUsage(999);
    DicomException("NO IMAGE TO BE ANALYSED OF TYPE "+DicomVImage::GetModalityStr(theAnalyseModality));
  }

  //----- IMAGE WITH STRUCTURES
  //  G4int iUsePolygons = theParamMgr->GetNumericValue("UsePolygons",-1);
  // G4bool bUsePolygons = false; // default value
  //  if( iUsePolygons != -1 ) bUsePolygons == GmGenUtils::GetBool(iUsePolygons); // value set by user
  //--- IF fSt EXISTS USE IT FOR STRUCTURES
  if( !bPerMate ) {
    std::vector<DicomVReader*> VRTFiles = theReaderMgr->GetReaders(DRM_RTStruct, false);
    if( VRTFiles.size() == 1 ) { 
      DicomReaderRTStruct* RTFile = dynamic_cast<DicomReaderRTStruct*>(VRTFiles[0]);
      theStructIDImage = new DicomVImageStr( theAnalyseImage, "RTStruct", DIM_RTStruct );
      DicomPolygonSet* polygonSet = RTFile->GetPolygonSet();
      G4bool bVoxelsInUseClosest = DicomParameterMgr::GetInstance()->GetNumericValue("closestPolygon",1);
      if( bVoxelsInUseClosest ) {
	DicomPolygonSet* polygonSetNew = new DicomPolygonSet(polygonSet, theStructIDImage, polygonSet->GetOrientation() );
	RTFile->FindVoxelsInXY( theStructIDImage, polygonSetNew );
	delete polygonSetNew;
      } else {
	RTFile->FindVoxelsInXY( theStructIDImage, polygonSet );
      }	  

      //--- SET CLASSIFICATION NAMES
      DicomPolygonSet* thePolygonSet = RTFile->GetPolygonSet();
      theClassifNames = thePolygonSet->GetLineListNames();	
      theClassifNames[0] = "NO_STRUCT";
    }
  }
  //  G4cout << " GET g4dcmReader?? " << theParamMgr->GetVStringValue("fG4dcmCT","") << " " << bPerMate << " " <<theStructIDImage << G4endl; //GDEB
  
  std::vector<DicomVReader*> VPerCentFiles = theReaderMgr->GetReaders(DRM_G4dcmNM,false);
  if( VPerCentFiles.size() == 1 ) {
    DicomReaderG4dcmNM* g4dcmNMReader = dynamic_cast<DicomReaderG4dcmNM*>(VPerCentFiles[0]);
    thePerCentImage = g4dcmNMReader->GetImage();
  }
  //  G4cout << " DRM_G4dcmNM " << VPerCentFiles.size() << " thePerCentImage " << thePerCentImage << G4endl; //GDEB


  std::vector<DicomVReader*> VG4dcmCTFiles = theReaderMgr->GetReaders(DRM_G4dcmCT,false);
  if( VG4dcmCTFiles.size() == 1 ) {
    DicomReaderG4dcmCT* g4dcmReader = dynamic_cast<DicomReaderG4dcmCT*>(VG4dcmCTFiles[0]);
    //    G4cout << " GET g4dcmReader " << bPerMate << " " <<theStructIDImage << G4endl; //GDEB
    //--- IF fSt DOES NOT EXIST TAKE Structures from G4dcmCT
    if( !bPerMate ) {
      if( !theStructIDImage ) { // Not read already from fSt
	if( g4dcmReader->HasStructID()  ) {
	  theStructIDImage = g4dcmReader->GetStructIDImage(); // select fSt first
	  if( DicomVerb(testVerb) ){
	    G4cout << " ReadFilesAndGetImages STRUCT IMAGE FROM G4dcmCT " << theStructIDImage << G4endl;
	    theStructIDImage->DumpHeaderToTextFile( G4cout );
	  }

	  //--- SET CLASSIFICATION NAMES 
	  theClassifNames = g4dcmReader->GetStructNames();
	  theClassifNames[0] = "NO_STRUCT";
	} else {
	  DicomException("CANNOT ASK TO CALCULATE Dose/DepoEnergy IF NO FILE WITH STRUCTURE IS GIVEN");
	}
      }
    } else {
      //t      theStructIDImage = g4dcmReader->GetMateIDImage(); // select fSt first
      theClassifNames = g4dcmReader->GetMaterialNames();
    }
  }
    
  if( !theStructIDImage ) {
    DicomException("NO FILE WITH STRUCTURES SELECTED");
  }
  
  if( thePrintStName != "" ) {
    for( std::map<G4int,G4String>::const_iterator ite = theClassifNames.begin(); ite != theClassifNames.end(); ite++ ) {
      if( thePrintStName == ite->second ) {
	thePrintStID = ite->first;
      }
    }
    if( thePrintStID == -1 ) {
      G4Exception(theExeName.c_str(),
		  "",
		  FatalException,
		  ("Structure name not found : " + thePrintStName).c_str());
    }
  }
  
}

//------------------------------------------------------------------------------------
void DCMGetOrganInfo::ReadLinesDefinition()
{
  //----- READ StLines TO GET LINE TYPES AND COLORS
  theStLinesFN = theParamMgr->GetStringValue("fStLines","");
  if( theStLinesFN == "" ) return;
  
  if( DicomVerb(infoVerb) ) G4cout << "@@@@@@@ Reading STRUCTURES TO DRAW LINES: " << theStLinesFN << G4endl;
  GmFileIn fin = GmFileIn::GetInstance(theStLinesFN);
  std::vector<G4String> wl;
  for(;;) {
    if( !fin.GetWordsInLine(wl) ) break;
    if( wl.size() < 2 || wl.size() > 5 ) {
      G4Exception(theExeName.c_str(),
		  "Reading STRUCTURES TO DRAW file",
		  FatalException,
		  "Line must have two words : STRUCTURE_NAME DRAWING_OPTION (COLOR) (LINE_STYLE) (LINE_WIDTH)");
    }
    theStLines[wl[0]] = GmGenUtils::GetInteger(wl[1]); // 0: no drawing, 1: draw line, 2: draw also value 
    if( wl.size() >= 3 ) theStColor[wl[0]] = GmGenUtils::GetInteger(wl[2]);
    if( wl.size() >= 4 ) theStLineStyle[wl[0]] = GmGenUtils::GetInteger(wl[3]);
    if( wl.size() >= 5 ) theStLineWidth[wl[0]] = GmGenUtils::GetValue(wl[4]);
  }
  theNColorsInFile += theStColor.size();
  
}

//------------------------------------------------------------------------------------
void DCMGetOrganInfo::GetInfoFromImage()
{
  //--- GET GEOMETRY DATA
  size_t nVoxelX_st = theStructIDImage->GetNoVoxelsX();
  size_t nVoxelY_st = theStructIDImage->GetNoVoxelsY();
  size_t nVoxelXY_st = nVoxelX_st*nVoxelY_st;
  size_t nVoxelZ_st = theStructIDImage->GetNoVoxelsZ();
  float fWidthX_st = theStructIDImage->GetVoxelDimX();
  float fWidthY_st = theStructIDImage->GetVoxelDimY();
  float fWidthZ_st = theStructIDImage->GetVoxelDimZ();
  G4double fMinX_st = theStructIDImage->GetMinX();
  G4double fMinY_st = theStructIDImage->GetMinY();
  G4double fMinZ_st = theStructIDImage->GetMinZ();
  theStructIDImage->DumpHeaderToTextFile( G4cout ); 

  size_t nVoxelX_anal = theAnalyseImage->GetNoVoxelsX();
  size_t nVoxelY_anal = theAnalyseImage->GetNoVoxelsY();
  size_t nVoxelXY_anal = nVoxelX_anal*nVoxelY_anal;
  size_t nVoxelZ_anal = theAnalyseImage->GetNoVoxelsZ();
  size_t nVoxelXYZ_anal = nVoxelXY_anal*nVoxelZ_anal;
  float fWidthX_anal = theAnalyseImage->GetVoxelDimX();
  float fWidthY_anal = theAnalyseImage->GetVoxelDimY();
  float fWidthZ_anal = theAnalyseImage->GetVoxelDimZ();
  G4double fMinX_anal = theAnalyseImage->GetMinX();
  G4double fMinY_anal = theAnalyseImage->GetMinY();
  G4double fMinZ_anal = theAnalyseImage->GetMinZ();
  theAnalyseImage->PrintHeader(); //GDEB
  theStructIDImage->PrintHeader(); //GDEB
  
  //----- CHECK IF ANALYSE IMAGE AND STRUCTURE IMAGE HAVE SAME VOXELS
  bDiffDim = 0;
  if( nVoxelX_st != nVoxelX_anal
      || nVoxelY_st != nVoxelY_anal
      || nVoxelZ_st != nVoxelZ_anal ) {
    G4cerr << "STRUCTURE N VOXELS = " << nVoxelX_st << " " << nVoxelY_st << " " << nVoxelZ_st << G4endl
	   << "ANALYSE N VOXELS= " << nVoxelX_anal << " " << nVoxelY_anal << " " << nVoxelZ_anal << G4endl;
    G4Exception(theExeName,
		"Error",
		JustWarning,
		"structure and analyse files have different number of voxels");
    bDiffDim = 1;
  }
  
  if( fabs(fWidthX_st - fWidthX_anal) > theAnalyseImage->GetPrecision() 
      || fabs(fWidthY_st - fWidthY_anal) > theAnalyseImage->GetPrecision() 
      || fabs(fWidthZ_st - fWidthZ_anal) > theAnalyseImage->GetPrecision()  ) {
    G4cerr << "STRUCTURE VOXEL DIMS = " << fWidthX_st << " " << fWidthY_st << " " << fWidthZ_st << G4endl
	   << "ANALYSE VOXEL DIMS= " << fWidthX_anal << " " << fWidthY_anal << " " << fWidthZ_anal << G4endl
	   << " DIFF " << fabs(fWidthX_st - fWidthX_anal) << " " << fabs(fWidthY_st - fWidthY_anal) << " " << fabs(fWidthZ_st - fWidthZ_anal) << " < " << theAnalyseImage->GetPrecision() << G4endl;
    
    G4Exception(theExeName,
		"Error",
		JustWarning,
		"structure and analyse files have different voxel dimensions");
    bDiffDim = 1;
  }
  if( fabs(fMinX_st - fMinX_anal) > theAnalyseImage->GetPrecision()
      || fabs(fMinY_st - fMinY_anal) > theAnalyseImage->GetPrecision()
      || fabs(fMinZ_st - fMinZ_anal) > theAnalyseImage->GetPrecision() ) {
    G4cerr << "STRUCTURE VOXEL MIN = " << fMinX_st << " " << fMinY_st << " " << fMinZ_st << G4endl
	   << "ANALYSE VOXEL MIN= " << fMinX_anal << " " << fMinY_anal << " " << fMinZ_anal << G4endl
      	   << " DIFF " << fabs(fMinX_st - fMinX_anal) << " " << fabs(fMinY_st - fMinY_anal) << " " << fabs(fMinZ_st - fMinZ_anal) << " < " << theAnalyseImage->GetPrecision() << G4endl;

    std::ofstream fo("bad.txt");
    theAnalyseImage->DumpHeaderToTextFile(fo);
    
    G4Exception(theExeName,
		"Error",
		JustWarning,
		"structure and analyse files have different minimum position");
    bDiffDim = 1;
  }
  
  //--- DEFINE DOSE & DOSE-VOLUME HISTOGRAM LIMITS
  std::vector<G4double>* theDoses = theAnalyseImage->GetData();
  TFile* histosFile = 0;
  if( bHistos ) {
    G4String hfn = theParamMgr->GetStringValue("fOut","")+".root";
    histosFile = new TFile(hfn.c_str(),"RECREATE");
    //    G4cout << " N theDoses " << theDoses->size() << " " << theAnalyseImage->GetModality() << G4endl; //GDEB
    //--- If not set, use limits from theAnalyseImage
    G4double doseMin = DBL_MAX;
    G4double doseMax = -DBL_MAX;
    if( !bUserHistoDoseMin || !bUserHistoDoseMax ) {
      G4double* doseP = &(theDoses->at(0));
      for( size_t ii = 0; ii < nVoxelXYZ_anal; ii++, doseP++){
	G4double dose = *doseP;
	if( dose == 0) continue;
	if( !bUserHistoDoseMin ) doseMin = min(doseMin,dose);
	if( !bUserHistoDoseMax ) doseMax = max(doseMax,dose);
	// if( ii%100000 == 0 ) G4cout << bUserHistoDoseMin << " " << bUserHistoDoseMax << " HISTOminmax DOSE " << dose << " MIN " << doseMin << " MAX " << doseMax << G4endl;  //GDEB
      }
      //increase the margins slightly
      if( !bUserHistoDoseMin ) {
	if( bLog ) {
	  theHistoDoseMin = GmGenUtils::GetBelowInt(log10(doseMin))-1;
	} else {
	  if( bUserHistoDoseMax ) {
	    theHistoDoseMin = max(0.,doseMin-(theHistoDoseMax-doseMin)/10.);
	  } else {
	    theHistoDoseMin = max(0.,doseMin-(doseMax-doseMin)/10.);
	  }
	}
      }
      //      std::cout << " " << doseMin << " " << doseMax << " bUserHistoDoseMin " << bUserHistoDoseMin << " : " << theHistoDoseMin << " " << bLog << G4endl; //GDEB
      if( !bUserHistoDoseMax ) {
	if( bLog ) {
	  theHistoDoseMax = GmGenUtils::GetAboveInt(log10(doseMax))+1;
	} else {
	  if( bUserHistoDoseMin ) {
	    theHistoDoseMax = doseMax+(doseMax-theHistoDoseMin)/10.;
	  } else {
	    theHistoDoseMax = doseMax+(doseMax-doseMin)/10.;
	  }
	}
      }
      if( DicomVerb(infoVerb) ) G4cout<< bUserHistoDoseMin << " MINIMUM DOSE " << theHistoDoseMin << " " << doseMin << " log10: " << int(log10(doseMin))-1 <<G4endl; 
      if( DicomVerb(infoVerb) ) G4cout<< bUserHistoDoseMax << " MAXIMUM DOSE " << theHistoDoseMax << " " << doseMax << " log10: " << int(log10(doseMax))+1 << G4endl; 
      
    }
    theAnaMgr = GmAnalysisMgr::GetInstance(theOutputName);
    
  }

  //----- CALCULATE theDosesPerSt . ALSO theVoxelDoseErrPerSt FOR DOSIMETRIC QUANTITIES
  std::map<size_t, int> theNVoxelsPerSt;  // number of voxels in each structure
  std::map<size_t, int> theNVoxels0PerSt;  // number of voxels in each structure with dose <> 0
  std::map<size_t, long double> theDoseSqsPerSt;
  std::map<size_t, long double> theMassesPerSt; //=the Densities, as all voxels are assumed to have the same volume
  std::map<size_t,std::map<size_t,mmdd> > theVoxelDoseErrPerSt;  // ist, iRandom, dose and dose err sorted in dose 
  
  std::vector<G4double>* theDoseErrors = 0;
  if( theAnalyseErrorImage ){
    theDoseErrors = theAnalyseErrorImage->GetData();
  }
  std::vector<G4double>* theDoseSqs = 0;
  if( theAnalyseSqImage ){
    theDoseSqs = theAnalyseSqImage->GetData();
  }
  G4int nDoseNon0 = 0;
  G4String magnitude = "Dose";
  if( bDepoEnergy ) magnitude = "EDepo";
  G4double* doseP = &(theDoses->at(0));
  G4double* doseErrP = 0;
  if( theDoseErrors ) doseErrP = &(theDoseErrors->at(0));
  G4double* doseSqP = 0;
  if( theDoseSqs ) doseSqP = &(theDoseSqs->at(0));
  G4double dose;
  G4double doseErr = 0.;
  G4double doseSq;
  std::set<G4int> ists;
  theVoxelVolume_anal = fWidthX_anal*fWidthY_anal*fWidthZ_anal;
  //  G4double voxelVolume_st = fWidthX_st*fWidthY_st*fWidthZ_st;
  
  for( size_t ii = 0; ii < nVoxelXYZ_anal; ii++, doseP++, doseErrP++, doseSqP++){
    dose = *doseP;
    if( theDoseErrors ) {
      if( theDoseErrors ) doseErr = *doseErrP;
      if( theDoseSqs ) doseSq = *doseSqP;
    }
      //    G4cout << " DOSEERR " << doseErr << " " << theDoseErrors << G4endl; //GDEB
    ists.clear();
    size_t iiSt = ii;
    if( bDiffDim ) { // sqdose and dcm have different voxels, interpolate
      G4int ix_anal = ii%nVoxelX_anal;
      G4int iy_anal = (ii/nVoxelX_anal)%nVoxelY_anal;
      G4int iz_anal = ii/nVoxelXY_anal;
      G4double X_anal = fMinX_anal+fWidthX_anal*(ix_anal+0.5);
      G4double Y_anal = fMinY_anal+fWidthY_anal*(iy_anal+0.5);
      G4double Z_anal = fMinZ_anal+fWidthZ_anal*(iz_anal+0.5);
      G4int ix_st = GmGenUtils::GetBelowInt((X_anal-fMinX_st)/fWidthX_st-0.01);
      G4int iy_st = GmGenUtils::GetBelowInt((Y_anal-fMinY_st)/fWidthY_st-0.01);
      G4int iz_st = GmGenUtils::GetBelowInt((Z_anal-fMinZ_st)/fWidthZ_st-0.01);
      if( ix_st < 0 || ix_st >= G4int(nVoxelX_st) || 
	  iy_st < 0 || iy_st >= G4int(nVoxelY_st) ||
	  iz_st < 0 || iz_st >= G4int(nVoxelZ_st) ) {
	continue;
      }
      iiSt = ix_st +iy_st*nVoxelX_st+iz_st*nVoxelXY_st;
      //??      dose = theDoses->at(iiSt);
    }
    if( bPerMate ) {
      ists.insert( theStructIDImage->GetData(iiSt) ); //using MateID from G4dcmCT
    } else {
      ists = theStructIDImage->GetIDList( iiSt );
    }
    if( dose != 0. ) {
      nDoseNon0++;
    }
    for( std::set<G4int>::const_iterator itest = ists.begin(); itest != ists.end(); itest++ ) {
      size_t ist = *itest;
      if( G4int(ist) == thePrintStID ) {
	G4cout << ist << " : " << theClassifNames[ist] << " " << ii << " VALUE: " << dose << " +- " << doseErr << G4endl; 
      }
      theNVoxelsPerSt[ist]++;
      if( dose != 0. ) {
	theNVoxels0PerSt[ist]++;
      }
      // not needed, because it is also not used in theMassesPerSt      dens *= theROIs[ist]->GetVolume(ii);
      double dens = theAnalyseMateDensImage->GetData(iiSt);
      if( bDosimQ ) {
	//	if( ii%100000 == 0 ) G4cout << ist << " : 0 " << " theVoxelDoseErrPerSt " << ii << " " << dose*theNofEvents << " +- " <<doseErr*theNofEvents << " dose " << dose << G4endl; //GDEB
	theVoxelDoseErrPerSt[ist][0].insert( mmdd::value_type(dose*theNofEvents, doseErr*theNofEvents) );
	//	if( ist == 3 ) G4cout << "0 theVoxelDosePerSt " << ist << " " <<  dose*theNofEvents << " N=" << theVoxelDoseErrPerSt[ist][0].size() << G4endl; //GDEB
      }
      if( dose != 0 ) {
	// if stored is energy deposited do not multiply
       	dose = *doseP * dens * theNofEvents;
	doseSq *= dens * theNofEvents * dens * theNofEvents;
	doseErr *= dens * theNofEvents;	
	//	if ( ist == 13 || ist == 14 ) G4cout << ii << " " << iiSt << " : " << ist << " FINAL IST DOSE " << theDosesPerSt[ist] << " " << dose << G4endl; //GDEB
	theDosesPerSt[ist] += dose; //multiply by volume to get energy, not dose
	theDoseSqsPerSt[ist] += doseSq; //multiply by volume to get energy, not dose
	G4int nCounts = theNVoxelsPerSt[ist];	
	//	if( ist == 4 ) G4cout << " DOSE ST dose "  << dose/(dens*theNofEvents) << "edep " <<  dose << " -> " << theDosesPerSt[ist] << " dens " <<dens <<  G4endl; 
	nCounts = 1;
	if( DicomVerb(testVerb) ) G4cout << " DOSE " << dose << " -> " << theDosesPerSt[ist] << " dose*dose " << dose*dose << " sq " << doseSq << " -> " << theDoseSqsPerSt[ist] << " DIFF " << doseSq - dose*dose << G4endl; 
	if( nCounts != 1 ) {
	  theDoseErrorsPerSt[ist] = ((theDoseSqsPerSt[ist])*nCounts - theDosesPerSt[ist]*theDosesPerSt[ist]) / (nCounts-1);
	} else{
	  theDoseErrorsPerSt[ist] = doseErr*doseErr;
	}
	if( DicomVerb(testVerb) ) G4cout<< ii << " theDosesPerSt " << ist << " " << dose << " +- " << doseErr << " -> " << theDosesPerSt[ist] << " +- " << sqrt(theDoseErrorsPerSt[ist]) << " sq " <<  theDoseSqsPerSt[ist] << " : " << (theDoseSqsPerSt[ist])*nCounts << " - " << theDosesPerSt[ist]*theDosesPerSt[ist] << " = " << ((theDoseSqsPerSt[ist])*nCounts - theDosesPerSt[ist]*theDosesPerSt[ist]) << " dens " << dens << " nCounts " <<nCounts  << G4endl;
	//	  theDoseErrorsPerSt[ist] = doseErr*doseErr;
      }
      if( !bDepoEnergy ) { // dose: voxel masses summed up for dividing sum of voxel doses
	theMassesPerSt[ist] += dens; 
	//	theMassesPerSt[ist] += 1./(dens/CLHEP::joule/(theVoxelVolume_anal*1e-6)); //if stored is energy deposited
	//	if( ist == 132 ) G4cout << 1./CLHEP::joule << " / " << theVoxelVolume_anal*1e-6 << " MASS ST " << dens/CLHEP::joule/(theVoxelVolume_anal*1e-6) << " -> " << theMassesPerSt[ist] << " mass " << dens << G4endl; 
      }

    }
  }

  //----- DIVIDE E_DEPOSITED BY MASS OR CONVERT E_DEPOSITED TO MeV
  if( DicomVerb(infoVerb) ) G4cout << " DOSE/EDEP theDosesPerSt SIZE " << theDosesPerSt.size() << G4endl; 
  std::map<size_t, long double>::const_iterator ited;
  G4double edepFactor = theVoxelVolume_anal * CLHEP::g/CLHEP::kg * CLHEP::mm3/CLHEP::cm3 * CLHEP::joule;
  //  G4cout << " EDEPFACTOR " << edepFactor << " = " << theVoxelVolume_anal << " * " << CLHEP::g/CLHEP::kg << " * " << CLHEP::mm3/CLHEP::cm3 << " * " << CLHEP::joule << G4endl; //GDEB

  for( ited = theDosesPerSt.begin(); ited != theDosesPerSt.end(); ited++){
     size_t id = (*ited).first;
     theDoseErrorsPerSt[id] = sqrt(theDoseErrorsPerSt[id]);
     if( bDepoEnergy ) {
       theDosesPerSt[id] *= edepFactor;
       theDoseErrorsPerSt[id] *= edepFactor;
     } else { // DOSE: DIVIDE IT BY TOTAL St MASS
       G4double mass = theMassesPerSt[id];
       if( mass != 0 ) {
	// if stored is energy deposited
	 theDosesPerSt[id] /= mass;
	 theDoseErrorsPerSt[id] /= mass; 
	 if( DicomVerb(testVerb) ) G4cout << " theDosesPerSt/=mass " << id << " " << dose << " +- " << doseErr << " -> " << theDosesPerSt[id] << " +- " << sqrt(theDoseErrorsPerSt[id]) << " mass " << mass << G4endl;
       }
     }
  }

  //----- PRINT IN FILE DOSE/EDEP PER STRUCTURE/MATERIAL AND CALCULATE TOTALS
  G4double doseTotal = 0.;
  G4double errTotal = 0.;
  G4int nVoxTotal = 0;
  for( ited = theDosesPerSt.begin(); ited != theDosesPerSt.end(); ited++){
    size_t id = (*ited).first;
    G4double val = theDosesPerSt[id];
    G4double err = theDoseErrorsPerSt[id];
    doseTotal += val;
    errTotal += err*err;
    if ( !bPerMate && id != 0 ) nVoxTotal += theNVoxelsPerSt[id]; // materials start with 0, structures with 1
    if( DicomVerb(infoVerb) ) {
      if( bDepoEnergy ) {
	G4cout << id << " EDEP ";
      } else {
	G4cout << id << " DOSE ";
      }
      G4cout << theClassifNames[id];
      G4cout << " = " << val;
      if( bDepoEnergy ) {
	G4cout << " MeV ";
      } else {
	G4cout << " Gray ";
      }
      G4cout << " +-(REL) " << err/val << " N= " << theNVoxelsPerSt[id] << " Nnon0= " << theNVoxels0PerSt[id] << G4endl;
    }
  }
  errTotal = sqrt(errTotal);
  //----- PRINT TOTALS
  if( DicomVerb(infoVerb) ) {
    G4cout << " TOTAL " << doseTotal;
    if( bDepoEnergy ) {
      G4cout << " MeV ";
    } else {
      G4cout << " Gray ";
    }
    G4cout << " +- " << errTotal << " N= " << nVoxTotal << G4endl;
    G4cout << " N_VOXELS_NOT_0 " << nDoseNon0 <<  " OUT_OF " << nVoxelXYZ_anal << " %= " << float(nDoseNon0)/nVoxelXYZ_anal <<  G4endl;
  }

  //----- WRITE IN FILE AS %
  //--- WIDTH STRUCTURE/MATERIAL NAMES
  std::map<G4int,G4String>::const_iterator itec;
  size_t maxNameWidth = 0;
  size_t maxNumberWidth = 0;
  for( itec = theClassifNames.begin(); itec != theClassifNames.end(); itec++ )  {
    maxNameWidth = max(maxNameWidth,(*itec).second.length()); 
    maxNumberWidth = max(maxNumberWidth,size_t((*itec).first));
    //    G4cout << "maxNameWidth " << maxNameWidth << " : " << (*itec).second.length()  << G4endl; //GDEB
    //        G4cout << "maxNumberWidth " << maxNumberWidth << " : " << (*itec).first  << G4endl; //GDEB
    if( DicomVerb(testVerb) ) G4cout << (*itec).second << " STRUCTURE/MATERIAL " << (*itec).second << G4endl; 
  }
  maxNumberWidth = log10(maxNumberWidth)+2;
  //  G4cout << "maxNumberWidth " << maxNumberWidth << G4endl; //GDEB

  std::ofstream fout(theParamMgr->GetStringValue("fOut","").c_str());
  fout << std::setw(maxNumberWidth) << "ID" 
       << std::setw(maxNameWidth+3) << "NAME"
       << " : DOSE per event (Gy) +- %Rel.Error : %DOSE +- %Rel.Error        : %_VOXELS=    "
       << " NVoxels   NVoxels_not0dose";
  if( !bPerMate ) {
    fout << " : MASS (g) ";
  }
  fout << G4endl;
  
  for( ited = theDosesPerSt.begin(); ited != theDosesPerSt.end(); ited++){
    size_t id = (*ited).first;
    G4double val = theDosesPerSt[id];
    G4double err = theDoseErrorsPerSt[id];
    if( val != 0 ) {
      float valr = val/doseTotal;
      float ev = err/val;
      float evt = errTotal/doseTotal;
      float errr = valr * sqrt(ev*ev+evt*evt); // * sqrt(1-valr);
      //      float errr = valr * sqrt(err/val*err/val+errTotal/doseTotal+errTotal/doseTotal);
      float nVoxelr = float(theNVoxelsPerSt[id])/nVoxTotal;
      // fout q<< theClassifNames[id] << " " << ev << " " << evt  << " " << errr/valr << " : " << sqrt(ev*ev+evt*evt) << " " << sqrt(ev*ev) << G4endl;
      //      G4cout << "FOUT err " << id  << " " << ev << " " << evt  << " " << errr/valr << " : " << sqrt(ev*ev+evt*evt) << " " << sqrt(ev*ev) << G4endl; 
      fout << std::setw(5) << id << " "; // SET WIDTH AUTOMATICALL //t
      fout << std::setw(maxNameWidth+2) << "\""+theClassifNames[id]+"\"";
      fout << " : " << val << " +-(%REL) " << ev*100 << " : " << valr*100. << " +-(%REL) " << errr*100 << " : %_VOXELS= " << nVoxelr*100
	   << " NVox= " << theNVoxelsPerSt[id] << " NVoxnon0= " << theNVoxels0PerSt[id];
      if( !bPerMate ) {
	fout << " : MASS= " << theMassesPerSt[id]*theVoxelVolume_anal/CLHEP::cm3 << " g";
      }
      fout << G4endl;
    }
  }
  fout << " TOTAL DOSE " << doseTotal;
  if( bDepoEnergy ) {
    fout << " MeV ";
  } else {
    fout << " Gray ";
  }
  fout << " +- " << errTotal << " N= " << nVoxTotal << G4endl;
  fout << " N_VOXELS_NOT_0 " << nDoseNon0 <<  " OUT_OF " << nVoxelXYZ_anal << " = " << float(nDoseNon0)/nVoxelXYZ_anal <<  G4endl;

  //----- @@@@ CALCULATE theVoxelDoseErrPerSt nRandom TIMES FOR DOSIM QUANTITIES
  G4int nRandom = GmParameterMgr::GetInstance()->GetNumericValue("nRandomizeError",0);
  if( nRandom > 0 ) {
    bRandom = true;
  }
  if( G4bool(GmParameterMgr::GetInstance()->GetNumericValue("plusError",0)) ) {
    nRandom = 1;
    bRandom = false;
  }
  if( G4bool(GmParameterMgr::GetInstance()->GetNumericValue("minusError",0)) ) {
    if( G4bool(GmParameterMgr::GetInstance()->GetNumericValue("plusError",0)) ) {
            G4Exception("DCMGetOrganInfo",
		  "",
		  FatalErrorInArgument,
			"You cannot set -minusError and -plusError to 1 simultaneously");
    }
    nRandom = 1;
    bRandom = false;
    theNErrorSigmas *= -1;
  }
  if( nRandom > 0 && theAnalyseErrorImage ) {
    if( bDosimQ || bHistos ) {
      if( DicomVerb(infoVerb) ){
	G4cout << " NRANDOM " << nRandom << " " << theAnalyseErrorImage << G4endl; 
      }
      
      if( theNErrorSigmas < 0 ) nRandom = 1;

      Dicom2DOperAddError* operAddErr = 0;
      if( bRandom ) {
	operAddErr = new Dicom2DOperAddError(-1,"Dicom2DOperAddError",theNErrorSigmas);
      } 
	
      DicomVImage* imageNew = new DicomVImage(*theAnalyseImage);
      
      for( G4int ir = 0; ir < nRandom; ir++ ) {
	if( bRandom ) {
	  //	  imageNew = new DicomVImage(*theAnalyseImage);
	  *imageNew = *theAnalyseImage;
	  operAddErr->Operate(imageNew, theAnalyseErrorImage);
	  if( DicomVerb(testVerb) ){
	    for( size_t iim = 0; iim < theAnalyseImage->GetData()->size(); iim += 100 ) {
	      G4cout << ir << " NRANDOM imageNew " << iim << " " << imageNew->GetData(iim) << " <- " << theAnalyseImage->GetData(iim) << " +- " << theAnalyseErrorImage->GetData(iim) << G4endl;
	    }
	  }
	} else {
	  std::vector<DicomVImage*> errImages;
	  errImages.push_back(theAnalyseImage);
	  errImages.push_back(theAnalyseErrorImage);
	  //	  imageNew = new DicomVImage(theAnalyseImage,"MinMaxErr",theAnalyseImage->GetModality());
	  *imageNew = *theAnalyseImage;
	  if( ir == 0 ) {
	    DicomNDOperArithm* operMinMaxErr = new DicomNDOperArithm(-1,"Dicom2DOperArithm","DATA1*"+GmGenUtils::ftoa(theNErrorSigmas)+"+DATA2*"+GmGenUtils::ftoa(theNErrorSigmas));
	    operMinMaxErr->Operate(imageNew, errImages);
	  } else if( ir == 1 ) {
	    DicomNDOperArithm* operMinMaxErr = new DicomNDOperArithm(-1,"Dicom2DOperArithm","DATA1*"+GmGenUtils::ftoa(theNErrorSigmas)+"-DATA2*"+GmGenUtils::ftoa(theNErrorSigmas));
	    operMinMaxErr->Operate(imageNew, errImages);
	  }
	}

	std::vector<G4double>* newDoses = imageNew->GetData();
	doseP = &(newDoses->at(0));
	doseErrP = &(theDoseErrors->at(0));
	doseErr = 0.;
	//  G4double voxelVolume_st = fWidthX_st*fWidthY_st*fWidthZ_st;
	
 	for( size_t ii = 0; ii < nVoxelXYZ_anal; ii++, doseP++, doseErrP++){
	  dose = *doseP;
	  doseErr = 0; //*doseErrP  not used: error is calculated in dosimQ as variation in the doses for each new image   
	  ists.clear();
	  size_t iiSt = ii;
	  if( bDiffDim ) { // sqdose and dcm have different voxels, interpolate
	    G4int ix_anal = ii%nVoxelX_anal;
	    G4int iy_anal = (ii/nVoxelX_anal)%nVoxelY_anal;
	    G4int iz_anal = ii/nVoxelXY_anal;
	    G4double X_anal = fMinX_anal+fWidthX_anal*(ix_anal+0.5);
	    G4double Y_anal = fMinY_anal+fWidthY_anal*(iy_anal+0.5);
	    G4double Z_anal = fMinZ_anal+fWidthZ_anal*(iz_anal+0.5);
	    G4int ix_st = GmGenUtils::GetBelowInt((X_anal-fMinX_st)/fWidthX_st-0.01);
	    G4int iy_st = GmGenUtils::GetBelowInt((Y_anal-fMinY_st)/fWidthY_st-0.01);
	    G4int iz_st = GmGenUtils::GetBelowInt((Z_anal-fMinZ_st)/fWidthZ_st-0.01);
	    if( ix_st < 0 || ix_st >= G4int(nVoxelX_st) || 
		iy_st < 0 || iy_st >= G4int(nVoxelY_st) ||
		iz_st < 0 || iz_st >= G4int(nVoxelZ_st) ) {
	      continue;
	    }
	    iiSt = ix_st +iy_st*nVoxelX_st+iz_st*nVoxelXY_st;
	  }
	  if( bPerMate ) {
	    ists.insert( theStructIDImage->GetData(iiSt) ); //using MateID from G4dcmCT
	  } else {
	    ists = theStructIDImage->GetIDList( iiSt );
	  }
	  for( std::set<G4int>::const_iterator itest = ists.begin(); itest != ists.end(); itest++ ) {
	    size_t ist = *itest;
 	    if( bDosimQ ) {
	      theVoxelDoseErrPerSt[ist][ir+1].insert( mmdd::value_type(dose*theNofEvents, doseErr*theNofEvents) );
	      //	      if( ii%100000 == 0 ) G4cout << ist << " : " << ir+1  << " theVoxelDoseErrPerSt " << ii << " " << dose*theNofEvents << " +- " <<doseErr*theNofEvents << " dose " << dose << G4endl; //GDEB
	      //	      if( ist == 3 ) G4cout << ir+1 << " theVoxelDosePerSt " << ist << " " << dose*theNofEvents << " N=" << theVoxelDoseErrPerSt[ist].size() << G4endl; //GDEB
	    }
	  }
	}
	if( ir == 0 ) theAnalyseImageRandom = new DicomVImage(*imageNew);
      }

      delete imageNew;
    } else {
      G4Exception("DCMGetOrganInfo",
		  "",
		  FatalErrorInArgument,
		  "Asking to calculate errors by recalculating images with voxel errors, but no histogram or dosimetric quantity is requested");
    }
  }

  if( bDosimQ ) {
    std::map<size_t,std::map<size_t,mmdd> >::const_iterator iteis;
    for( iteis = theVoxelDoseErrPerSt.begin(); iteis != theVoxelDoseErrPerSt.end(); iteis++ ) {
      size_t id = (*iteis).first;
      //      std::set<G4double> voxelDoseErrors1 = theDoseErrorsPerSt[id];
      std::map<size_t,mmdd> voxelDoseErrors11 = theVoxelDoseErrPerSt[id];
      for( size_t ii = 0; ii < theDosimQuantities.size(); ii++ ){
	//classify dosimetry quantity
	G4String dosimq = theDosimQuantities[ii];
	std::pair<G4double,G4double> dosimve;
	if( dosimq.substr(0,1) == "V" ) {
	  dosimve = GetDosimVolume(voxelDoseErrors11,dosimq, id);
	} else if( dosimq.substr(0,4) == "Dmin" ) {
	  //	  G4cout << " START  GetDosimDmincc" << id << " " << ii << G4endl; //GDEB
	  dosimve = GetDosimDmincc(voxelDoseErrors11,dosimq, id);
	} else if( dosimq.substr(0,4) == "Dmax" ) {
	  //	  G4cout << " START GetDosimDmaxcc " << id << " " << ii << G4endl; //GDEB
	  dosimve = GetDosimDmaxcc(voxelDoseErrors11,dosimq, id);
	} else if( dosimq == "Dmean" ) {
	  dosimve = GetDosimDmean(voxelDoseErrors11, id);
	  //	} else if( dosimq == "TOTD" ) {
	  //	  G4cout << " START GetDosimTOTD " << id << " " << ii << G4endl; //GDEB
	  //	  dosimve = GetDosimTOTD(id);
	} else if( dosimq.substr(0,1) == "D" ) {
	  //	  G4cout << " START GetDosimDPercentage " << id << " " << ii << G4endl; //GDEB
	  dosimve = GetDosimDPercentage(voxelDoseErrors11,dosimq, id);
	} else {
	  G4Exception("DCMGetOrganInfo",
		      "",
		      FatalException,
		      ("Dosimetry quantity not defined: " + dosimq
		       + "\n Available quantities are D*, Dmin*cc, Dmax*cc, V*").c_str());
	}
 	//	fout << "DOSIMETRY: " << std::setw(maxNameWidth+1) << "\"" << theClassifNames[id] << "\"";
	G4double dosimv = dosimve.first;
	G4double dosime = dosimve.second;
	std::cout << "DOSIMETRY: " << std::setw(maxNameWidth+2) << "\""+theClassifNames[id]+"\"";
	std::cout << " : " << dosimq << " = " << dosimv << " +- " << dosime << std::endl;
	fout << "DOSIMETRY: " << std::setw(maxNameWidth+2) << "\""+theClassifNames[id]+"\"";
	fout << " : " << dosimq << " = " << dosimv << " +- " << dosime << std::endl;
      }
    }
  }
      
  fout.close();

  //----- FILL DOSE & DOSE-VOLUME HISTOGRAMS
  pmhmh hpair;
  if( bHistos ) {
    if( theAnalyseImageRandom == 0) {
      hpair = FillDoseHistos(theAnalyseImage, thePerCentImage);
    } else {
      hpair = FillDoseHistos(theAnalyseImageRandom, thePerCentImage);
    }
    WriteHistosToFile( hpair, histosFile );

    if( DicomVerb(debugVerb) ){
      G4cout << " NRANDOM " << nRandom << " " << theAnalyseErrorImage << G4endl;
    }
    //    if( nRandom > 0 && theAnalyseErrorImage ) {
    if( nRandom > INT_MAX && theAnalyseErrorImage ) {

      if( theNErrorSigmas < 0 ) nRandom = 1;
      
      Dicom2DOperAddError* operAddErr = new Dicom2DOperAddError(-1,"Dicom2DOperAddError",theNErrorSigmas);
      
      //--- create TProfile copying TH1F
      std::map<size_t,TH1F*>::const_iterator iteh;
      mhp::const_iterator itehp;
      mhp doseHistosProf;
      //      G4cout << " NRANDOM hpairN " <<  hpair.first.size() << G4endl; //GDEB
      for( iteh = hpair.first.begin(); iteh != hpair.first.end(); iteh++ ) {
	TH1F* his = iteh->second;
	TAxis* axis = his->GetXaxis();
	std::string hisName = std::string(his->GetName())+"ERROR";
	TProfile* hisProf = new TProfile(hisName.c_str(), hisName.c_str(),axis->GetNbins(), axis->GetXmin(), axis->GetXmax());
	doseHistosProf[iteh->first] = hisProf;
      }
      //      G4cout << " doseHistosProf N " << doseHistosProf.size() << G4endl; //GDEB
      mhp doseVolumeHistosProf;
      for( iteh = hpair.second.begin(); iteh != hpair.second.end(); iteh++ ) {
	TH1F* his = iteh->second;
	TAxis* axis = his->GetXaxis();
	std::string hisName = std::string(his->GetName())+"ERROR";
	TProfile* hisProf = new TProfile(hisName.c_str(), hisName.c_str(),axis->GetNbins(), axis->GetXmin(), axis->GetXmax());
	doseVolumeHistosProf[iteh->first] = hisProf;
      }
     
      for( G4int ii = 0; ii < nRandom; ii++ ) {
	DicomVImage* imageNew = new DicomVImage(*theAnalyseImage);
	//  G4cout << ii << " NRANDOM imageNew " << imageNew << G4endl; 
	operAddErr->Operate(imageNew, theAnalyseErrorImage);
	hpair = FillDoseHistos(imageNew, thePerCentImage);
	
	//--- Fill profile histos with this random fluctuation
	for( iteh = hpair.second.begin(); iteh != hpair.second.end(); iteh++ ) {
	  TH1F* his = iteh->second;    
	  TAxis* axis = his->GetXaxis();
  	  itehp = doseVolumeHistosProf.find(iteh->first);
	  TProfile* hisProf = itehp->second;
	  for( int ih = 1; ih <= axis->GetNbins(); ih++ ) {
	    hisProf->Fill(axis->GetBinCenter(ih),his->GetBinContent(ih));
	    /*	    if( std::string(his->GetName()).find("TV") != std::string::npos ) {
&	      std::cout << ii << " " << ih << " NRANDOM hisProf " << hisProf->GetBinContent(ih) << " FROM " << his->GetBinContent(ih) << G4endl; //GDEB
}*/
	  }
	}
	delete imageNew;
      }
      pmhpmhp hpairp(doseHistosProf, doseVolumeHistosProf);
      WriteHistosToFile( hpairp, histosFile );
    } 
    histosFile->Close();
    
  }

  if( bDrawImageDoseInSt ) {
    DicomVImage* imageDoseInSt = FillImageDoseInSt( theDosesPerSt );
    DicomDrawerROOT* theDrawer = new DicomDrawerROOT();
    std::vector<DicomVLineSet*> theLineSets;
    G4String extraFileName = "";  
    theDrawer->DrawXY(imageDoseInSt,theLineSets,extraFileName);
    theDrawer->DrawXZ(imageDoseInSt,theLineSets,extraFileName);
    theDrawer->DrawYZ(imageDoseInSt,theLineSets,extraFileName);
  }

}
  
//---------------------------------------------------------------------------
std::pair< std::map<size_t,TH1F*>,std::map<size_t,TH1F*> > DCMGetOrganInfo::FillDoseHistos( DicomVImage* analyseImage, DicomVImage* voxelPerCentImage )
{
  mh doseHistos;
  mh doseVolumeHistos;
  std::set<G4int> ists;

  size_t nVoxelX_st = theStructIDImage->GetNoVoxelsX();
  size_t nVoxelY_st = theStructIDImage->GetNoVoxelsY();
  size_t nVoxelXY_st = nVoxelX_st*nVoxelY_st;
  size_t nVoxelZ_st = theStructIDImage->GetNoVoxelsZ();
  float fWidthX_st = theStructIDImage->GetVoxelDimX();
  float fWidthY_st = theStructIDImage->GetVoxelDimY();
  float fWidthZ_st = theStructIDImage->GetVoxelDimZ();
  G4double fMinX_st = theStructIDImage->GetMinX();
  G4double fMinY_st = theStructIDImage->GetMinY();
  G4double fMinZ_st = theStructIDImage->GetMinZ();

  size_t nVoxelX_anal = analyseImage->GetNoVoxelsX();
  size_t nVoxelY_anal = analyseImage->GetNoVoxelsY();
  size_t nVoxelXY_anal = nVoxelX_anal*nVoxelY_anal;
  size_t nVoxelZ_anal = analyseImage->GetNoVoxelsZ();
  size_t nVoxelXYZ_anal = nVoxelXY_anal*nVoxelZ_anal;
  float fWidthX_anal = analyseImage->GetVoxelDimX();
  float fWidthY_anal = analyseImage->GetVoxelDimY();
  float fWidthZ_anal = analyseImage->GetVoxelDimZ();
  G4double fMinX_anal = analyseImage->GetMinX();
  G4double fMinY_anal = analyseImage->GetMinY();
  G4double fMinZ_anal = analyseImage->GetMinZ();
  
  std::vector<G4double>* theDoses = 0;
  theDoses = analyseImage->GetData();
  G4String magnitude = "Dose";
  if( bDepoEnergy ) magnitude = "EDepo";
  G4double* doseP = &(theDoses->at(0));
  G4double dose;

  for( size_t ii = 0; ii < nVoxelXYZ_anal; ii++, doseP++ ){
    dose = *doseP;
    ists.clear();
    size_t iiSt = ii;
    if( bDiffDim ) { // sqdose and dcm have different voxels, interpolate
      G4int ix_anal = ii%nVoxelX_anal;
      G4int iy_anal = (ii/nVoxelX_anal)%nVoxelY_anal;
      G4int iz_anal = ii/nVoxelXY_anal;
      G4double X_anal = fMinX_anal+fWidthX_anal*(ix_anal+0.5);
      G4double Y_anal = fMinY_anal+fWidthY_anal*(iy_anal+0.5);
      G4double Z_anal = fMinZ_anal+fWidthZ_anal*(iz_anal+0.5);
      G4int ix_st = GmGenUtils::GetBelowInt((X_anal-fMinX_st)/fWidthX_st-0.01);
      G4int iy_st = GmGenUtils::GetBelowInt((Y_anal-fMinY_st)/fWidthY_st-0.01);
      G4int iz_st = GmGenUtils::GetBelowInt((Z_anal-fMinZ_st)/fWidthZ_st-0.01);
      if( ix_st < 0 || ix_st >= G4int(nVoxelX_st) || 
	  iy_st < 0 || iy_st >= G4int(nVoxelY_st) ||
	  iz_st < 0 || iz_st >= G4int(nVoxelZ_st) ) {
	continue;
      }
      iiSt = ix_st +iy_st*nVoxelX_st+iz_st*nVoxelXY_st;
      //??      dose = theDoses->at(iiSt);
    }
    if( bPerMate ) {
      ists.insert( theStructIDImage->GetData(iiSt) ); //using MateID from G4dcmCT
    } else {
      ists = theStructIDImage->GetIDList( iiSt );
    }
    double dens = theAnalyseMateDensImage->GetData(iiSt);

    for( std::set<G4int>::const_iterator itest = ists.begin(); itest != ists.end(); itest++ ) {
      size_t ist = *itest;
      
      if( doseHistos.find(ist) == doseHistos.end() ) {
	G4String hisNameD;
	G4String hisNameDV;
	if( bLog ) {
	  hisNameD = "Log"+magnitude+"_";
	  hisNameDV = "Log"+magnitude+"-Volume_";
	} else {
	  hisNameD = ""+magnitude+"_";
	  hisNameDV = ""+magnitude+"-Volume_";
	}
	hisNameD += theClassifNames[ist];
	hisNameDV += theClassifNames[ist];
	theAnaMgr->CreateHisto1D(hisNameD,500,theHistoDoseMin,theHistoDoseMax,90000+ist*1000+1);
	theAnaMgr->CreateHisto1D(hisNameDV,500,theHistoDoseMin,theHistoDoseMax,90000+ist*1000+2);
	doseHistos[ist] = theAnaMgr->GetHisto1D(90000+ist*1000+1);
	doseVolumeHistos[ist] = theAnaMgr->GetHisto1D(90000+ist*1000+2);
	//	G4cout << ist << " "<< doseVolumeHistos.size() << " CREATE DVHisto " << hisNameDV << " 500 " << theHistoDoseMin << " " << theHistoDoseMax << " " << 90000+ist*1000+2<< G4endl; //GDEB
      }
      G4double log10dose = 0.; 
      if( !bDepoEnergy ) { 
	log10dose = dose;  // use dose for DVH
	G4double voxelVolume = theVoxelVolume_anal;
	if( voxelPerCentImage ) { // Edepo is only in a fraction of voxel, so dose is bigger
	  voxelVolume = voxelPerCentImage->GetData(ii);
	  if( dens*voxelVolume != 0. ) {
	    log10dose *= theVoxelVolume_anal/(dens*voxelVolume);
	  //	  G4cout << ii << " voxelVolume " << voxelVolume << " : " << dens*voxelVolume << G4endl; //GDEB
	  }
	}
      } else {
	G4double voxelVolume = theVoxelVolume_anal;
	if( voxelPerCentImage ) {
	  voxelVolume = voxelPerCentImage->GetData(ii);
	  //	  G4cout << ii << "voxelVolume " << voxelVolume << " " << voxelPerCentImage->GetData(ii) << G4endl; //GDEB
	}
	log10dose = dose*dens*voxelVolume;  // use Edep for DVH
      }
      if( bLog ) {
	if( log10dose != 0 ) log10dose = log10(log10dose);
      }
      TH1F* hisD = theAnaMgr->GetHisto1D(90000+ist*1000+1);
      hisD->Fill(log10dose);
      TH1F* hisDV = theAnaMgr->GetHisto1D(90000+ist*1000+2);
      for( G4int ih = 1; ih < hisDV->GetNbinsX(); ih++ ){
	//       	if( log10dose != 0 && ih != 0) G4cout << theClassifNames[ist] << " "<< ih << " FILLDV " <<  hisDV->GetBinLowEdge(ih)<<  "  < " << log10dose << G4endl; //GDEB
	if( hisDV->GetBinLowEdge(ih) < log10dose ) {
	  hisDV->SetBinContent(ih, hisDV->GetBinContent(ih)+1 );
	  //	  G4cout << theClassifNames[ist] << " "<< ih << " SetBinContent " << hisDV->GetBinContent(ih) << " at " << hisDV->GetBinLowEdge(ih) << " < " << log10dose << G4endl; //GDEB
	} else {
	  break;
	}
      }
    }
  }
  
  //--- NORMALIZE Dose-Volume HISTOS TO 100
  G4double doseTVmax = -DBL_MAX;
  std::map<size_t,TH1F*>::const_iterator ite;
  for( ite = doseVolumeHistos.begin(); ite != doseVolumeHistos.end(); ite++ ) {
    TH1F* hisDV = (*ite).second;
    for( G4int ih = hisDV->GetNbinsX()-1; ih > 0; ih-- ){
      //  if ( ih == 10 )  G4cout << " NORM hisDV " << hisDV->GetBinContent(ih) << " -> " << hisDV->GetBinContent(ih) * 100./ hisDV->GetBinContent(1) << G4endl;  //GDEB
      hisDV->SetBinContent(ih, hisDV->GetBinContent(ih) * 100./ hisDV->GetBinContent(1) );
      //      G4cout << " NORM DONE hisDV " << hisDV->GetBinContent(ih) << " " << hisDV->GetBinContent(1) << G4endl; //GDEB
      //--- GET MAXIMUM OF HISTOGRAMS DOSE-VOLUMES TO DEFINE xInit
      if( hisDV->GetBinContent(ih) >= 5 ) {
	doseTVmax = max(doseTVmax, hisDV->GetBinLowEdge(ih)+hisDV->GetBinWidth(ih));
      }
    }
    //    (*hisDV) *= 100./ hisDV->GetBinContent(1);
  }

  theCanvasSizeXY = 1000;
  G4double CSR = theCanvasSizeXY/1000.; // CANVAS SIZE RATIO
  theXInit0 = (doseTVmax-theHistoDoseMin)/(theHistoDoseMax-theHistoDoseMin)*850*CSR;
  
  pmhmh hpair = pmhmh(doseHistos,doseVolumeHistos);
  return hpair;

}


//-----------------------------------------------------------------------------
void DCMGetOrganInfo::WriteHistosToFile( pmhmh hpair, TFile* histosFile )
{
  mh doseHistos = hpair.first;
  mh doseVolumeHistos = hpair.second;
  //----- DRAW DOSE-VOLUME HISTOS
  gStyle->SetOptStat(0);
  TCanvas* canvasD = new TCanvas();
  canvasD->SetCanvasSize(theCanvasSizeXY,theCanvasSizeXY);
  TCanvas* canvas1 = new TCanvas();
  canvas1->SetCanvasSize(theCanvasSizeXY,theCanvasSizeXY);
  canvasD->SetRightMargin(0.2);
  canvasD->Draw();
  canvasD->cd();
  G4int ii;
  TH1F* hisMax = 0; //GET HISTO WITH MAXIMUM, TO SET THE LABEL Y POS
  G4double hismaxv = -DBL_MAX;
  std::map<size_t,TH1F*>::const_iterator ite;
  for( ite = doseHistos.begin(), ii=0; ite != doseHistos.end(); ite++,ii++ ) {
    G4double maxY = (*ite).second->GetMaximum();
    if(maxY > hismaxv ) {
      hismaxv = maxY;
      hisMax = (*ite).second;
    }
  }
  double xInit; 
  double yInit;
  G4double CSR = theCanvasSizeXY/1000.; // CANVAS SIZE RATIO
  for( ite = doseHistos.begin(), ii=0; ite != doseHistos.end(); ite++,ii++ ) {
    canvasD->cd();
    size_t ist = (*ite).first;
    TH1F* his = (*ite).second;
    if( bLog ) {
      his->SetXTitle("log_10(Dose) (Gy)");
    } else {
      his->SetXTitle("Dose (Gy)");
    }
    his->SetYTitle("N Voxels");
    if( ite == doseHistos.begin() ) {
      his->Draw("histo");
    } else {
      his->Draw("histoSames");
    }
    histosFile->cd();
    his->Write();
    G4String organName = theClassifNames[ist];
    EColor color = GetColor(ii+theNColorsInFile);
    //    EColor color = GetColor(ist);
    std::map<G4String,G4int>::const_iterator itecolor = theStColor.find(organName);
    if( itecolor != theStColor.end() ) {
      color = EColor((*itecolor).second);
    } else {
      if( theStColor.size() != 0 ) {
	G4Exception(theExeName.c_str(),
		    "",
		    JustWarning,("Color for line not found "+organName+", setting it to "+ GmGenUtils::itoa(color)).c_str());
      }
    }
    his->SetLineColor(color);
    xInit = theXInit0;
    yInit = (590 - ii*25)*CSR;
    if( yInit < 0 ) {
      yInit += 600*CSR;
      xInit = theXInit0;
    }
    DrawWordAndMarker( hisMax, organName, xInit, yInit, color, 0, 0.025, 0 );
    
    //--- draw each histogram in a file
    canvas1->Draw();
    canvas1->cd();
    his->Draw("histo");
    G4String hisNameD = theOutputName+"_Dose_";
    hisNameD += organName+".gif";    
    canvas1->Print(hisNameD.c_str());
  }
  canvasD->Print((theOutputName+"_Dose.ALL.gif").c_str());
  
  TCanvas* canvasDV = new TCanvas();
  canvasDV->SetRightMargin(0.25);
  canvasDV->SetLeftMargin(0.12);
  canvasDV->Draw();
  canvasDV->SetCanvasSize(theCanvasSizeXY,theCanvasSizeXY);
  for( ite = doseVolumeHistos.begin(), ii = 0; ite != doseVolumeHistos.end(); ite++, ii++ ) {    
    canvasDV->cd();
    size_t ist = (*ite).first;
    TH1F* his = (*ite).second;
    if( bLog ) {
      his->SetXTitle("log_10(Dose)");
    } else {
      his->SetXTitle("Dose");
    }
    his->SetYTitle("% N Voxels");
    if( ite == doseHistos.begin() ) {
      his->Draw("histo");
    } else {
      his->Draw("histoSames");
      }
    histosFile->cd();
    his->Write();
    if( DicomVerb(infoVerb) ){
      std::cout << " DRAW DOSE-VOLUME " << his->GetName() << " " << his->GetXaxis()->GetNbins() << " " <<  his->GetXaxis()->GetXmin() << " " << his->GetXaxis()->GetXmax() << G4endl;
    }
    G4String organName;
    organName = theClassifNames[ist];
    EColor color = GetColor(ii+theNColorsInFile);
    //    EColor color = GetColor(ist);
    std::map<G4String,G4int>::const_iterator itecolor = theStColor.find(organName);
    if( itecolor != theStColor.end() ) {
      color = EColor((*itecolor).second);
    } else {
      if( theStColor.size() != 0 ) {
	G4Exception(theExeName.c_str(),
		    "",
		    JustWarning,("Color for line not found "+organName+", setting it to "+ GmGenUtils::itoa(color)).c_str());
      }
    }
    his->SetLineColor(color);
    //    G4cout << " COLOR "  << organName << " " << color << " " << ii << G4endl; //GEB
    xInit = theXInit0;
    yInit = (590 - ii*22)*CSR;
    if( yInit < 0 ) {
      yInit += 600*CSR;
      xInit = theXInit0;
    }
    DrawWordAndMarker( his, organName, xInit, yInit, color, 0, 0.025, 0 );
    
    canvas1->Draw();
    canvas1->cd();
    his->Draw("histo");
    G4String hisNameDV;
    if( bLog ) {
      hisNameDV = theOutputName+"_LogDose-Volume_";
    } else {
      hisNameDV = theOutputName+"_Dose-Volume_";
    }
    hisNameDV += organName+".gif";
    canvas1->Print(hisNameDV.c_str());
  }
  canvasDV->Print((theOutputName+"_Dose-Volume.ALL.gif").c_str());

}

//-----------------------------------------------------------------------------
void DCMGetOrganInfo::WriteHistosToFile( pmhpmhp hpair, TFile* histosFile )
{

  gStyle->SetOptStat(1111111);
  mhp doseHistos = hpair.first;
  mhp doseVolumeHistos = hpair.second;
  //----- DRAW DOSE-VOLUME HISTOS
  gStyle->SetOptStat(0);
  TCanvas* canvasD = new TCanvas();
  canvasD->SetCanvasSize(theCanvasSizeXY,theCanvasSizeXY);
  TCanvas* canvas1 = new TCanvas();
  canvas1->SetCanvasSize(theCanvasSizeXY,theCanvasSizeXY);
  canvasD->SetRightMargin(0.2);
  canvasD->Draw();
  canvasD->cd();
  G4int ii;
  TProfile* hisMax = 0; //GET HISTO WITH MAXIMUM, TO SET THE LABEL Y POS
  G4double hismaxv = -DBL_MAX;
  mhp::const_iterator ite;
  for( ite = doseHistos.begin(), ii=0; ite != doseHistos.end(); ite++,ii++ ) {
    G4double maxY = (*ite).second->GetMaximum();
    if(maxY > hismaxv ) {
      hismaxv = maxY;
      hisMax = (*ite).second;
    }
  }
  //  G4cout << ii << " NRANDOM WriteHistosToFile " << doseHistos.size() << " maxY " << hismaxv << G4endl; //GDEB
  double xInit; 
  double yInit;
  G4double CSR = theCanvasSizeXY/1000.; // CANVAS SIZE RATIO
  for( ite = doseHistos.begin(), ii=0; ite != doseHistos.end(); ite++,ii++ ) {
    canvasD->cd();
    size_t ist = (*ite).first;
    TProfile* his = (*ite).second;
    if( bLog ) {
      his->SetXTitle("log_10(Dose) (Gy)");
    } else {
      his->SetXTitle("Dose (Gy)");
    }
    his->SetYTitle("N Voxels");
    if( ite == doseHistos.begin() ) {
      his->Draw("Ehisto");
    } else {
      his->Draw("EhistoSames");
    }
    histosFile->cd();
    his->Write();
    G4String organName = theClassifNames[ist];
    EColor color = GetColor(ii+theNColorsInFile);
    //    EColor color = GetColor(ist);
    std::map<G4String,G4int>::const_iterator itecolor = theStColor.find(organName);
    if( itecolor != theStColor.end() ) {
      color = EColor((*itecolor).second);
    } else {
      if( theStColor.size() != 0 ) {
	G4Exception(theExeName.c_str(),
		    "",
		    JustWarning,("Color for line not found "+organName+", setting it to "+ GmGenUtils::itoa(color)).c_str());
      }
    }
    his->SetLineColor(color);
    xInit = theXInit0;
    yInit = (590 - ii*25)*CSR;
    if( yInit < 0 ) {
      yInit += 600*CSR;
      xInit = theXInit0;
    }
    DrawWordAndMarker( hisMax, organName, xInit, yInit, color, 0, 0.025, 0 );
    
    //--- draw each histogram in a file
    canvas1->Draw();
    canvas1->cd();
    his->Draw("Ehisto");
    G4String hisNameD = theOutputName+"_Dose_ERR_";
    hisNameD += organName+".gif";    
    canvas1->Print(hisNameD.c_str());
  }
  //  G4cout << ii << " NRANDOM WriteHistosToFile " <<theOutputName <<"_Dose_ERR.ALL.gif "<< G4endl; //GDEB

  canvasD->Print((theOutputName+"_Dose_ERR.ALL.gif").c_str());
  
  TCanvas* canvasDV = new TCanvas();
  canvasDV->SetRightMargin(0.2);
  canvasDV->Draw();
  canvasDV->SetCanvasSize(theCanvasSizeXY,theCanvasSizeXY);
  for( ite = doseVolumeHistos.begin(), ii = 0; ite != doseVolumeHistos.end(); ite++, ii++ ) {    
    canvasDV->cd();
    size_t ist = (*ite).first;
    TProfile* his = (*ite).second;
    if( bLog ) {
      his->SetXTitle("log_10(Dose)");
    } else {
      his->SetXTitle("Dose");
    }
    his->SetYTitle("% N Voxels");
    if( ite == doseHistos.begin() ) {
      his->Draw("Ehisto");
    } else {
      his->Draw("EhistoSames");
    }
    histosFile->cd();
    his->Write();
    G4String organName;
    organName = theClassifNames[ist];
    EColor color = GetColor(ii+theNColorsInFile);
    //    EColor color = GetColor(ist);
    std::map<G4String,G4int>::const_iterator itecolor = theStColor.find(organName);
    if( itecolor != theStColor.end() ) {
      color = EColor((*itecolor).second);
    } else {
      if( theStColor.size() != 0 ) {
	G4Exception(theExeName.c_str(),
		    "",
		    JustWarning,("Color for line not found "+organName+", setting it to "+ GmGenUtils::itoa(color)).c_str());
      }
    }
    his->SetLineColor(color);
    //    G4cout << " COLOR "  << organName << " " << color << " " << ii << G4endl; //GEB
    xInit = theXInit0;
    yInit = (590 - ii*22)*CSR;
    if( yInit < 0 ) {
      yInit += 600*CSR;
      xInit = theXInit0;
    }
    DrawWordAndMarker( his, organName, xInit, yInit, color, 0, 0.025, 0 );
    
    canvas1->Draw();
    canvas1->cd();
    his->Draw("Ehisto");
    G4String hisNameDV;
    if( bLog ) {
      hisNameDV = theOutputName+"_LogDose-Volume_ERR_";
    } else {
      hisNameDV = theOutputName+"_Dose-Volume_ERR_";
    }
    hisNameDV += organName+".gif";
    canvas1->Print(hisNameDV.c_str());
  }
  canvasDV->Print((theOutputName+"_Dose-Volume_ERR.ALL.gif").c_str());

}

//---------------------------------------------------------------------------
DicomVImage* DCMGetOrganInfo::FillImageDoseInSt( std::map<size_t, long double> dosesPerSt )
{
  size_t nVoxelXYZ_st = theStructIDImage->GetNoVoxels();
  
  DicomVImage* imageDosePerSt = new DicomVImage(theStructIDImage,"DosePerSt",DIM_G4dcmDose);
  std::vector<G4double>* dosePerSt = imageDosePerSt->GetData();
  //  G4cout << " IMAGE DATA dosePerSt " << dosePerSt->size() << " nVoxelXYZ " << nVoxelXYZ_st << G4endl; //GDEB
  std::vector<DicomVReader*> VG4dcmCTFiles = theReaderMgr->GetReaders(DRM_G4dcmCT,true);
  std::map<G4int,G4String> theStructs = dynamic_cast<DicomReaderG4dcmCT*>(VG4dcmCTFiles[0])->GetStructNames();
  if( DicomVerb(debugVerb) ){
    for( std::map<G4int,G4String>::const_iterator ite= theStructs.begin(); ite != theStructs.end(); ite++ ) {
      G4cout << " STRUCTS " << ite->first << " : " << ite->second << G4endl; //GDEB
    }
  }

  //  size_t nVoxelX_st = theStructIDImage->GetNoVoxelsX(); //GDEB P
  // size_t nVoxelY_st = theStructIDImage->GetNoVoxelsY(); 
  //  size_t nVoxelXY_st = nVoxelX_st*nVoxelY_st;

  // LOOP TO CT VOXELS 
  std::set<G4int> ists;
  for( size_t ii = 0; ii < nVoxelXYZ_st; ii++){
    ists.clear();
    ists = theStructIDImage->GetIDList( ii );
    G4double dose = 0.;
    for( std::set<G4int>::const_iterator itest = ists.begin(); itest != ists.end(); itest++ ) {//--- LOOP TO STRUCTURES OF VOXEL
      size_t ist = *itest;
      G4double dose1 = dosesPerSt[ist];
      G4String stName = theStructs[ist];
      G4bool bExclude = false;
      for( size_t nn = 0; nn < theExcludeStNames.size(); nn++ ) {
	if( GmGenUtils::AreWordsEquivalent(theExcludeStNames[nn],stName ) ) {
	  bExclude = true;
	  break;
	}
      }
      if( bExclude ) dose1 = 0.;
      /*      size_t ix = ii%nVoxelX_st;
      size_t iy = (ii/nVoxelX_st)%nVoxelY_st;
      size_t iz = ii/nVoxelXY_st;
      G4cout << "DOSEST " << ix << " " << iy << " " << iz << " " << dose1 << " \"" << stName << "\"" << G4endl; //GDEB */
      
      //      G4cout << ii << " " << ist << " dosesPerSt " << dosesPerSt.size() << G4endl; //GDEB
      if( bHistosLogN ) {
	G4double logFactor = log(theHistosLogN);
	if( dose1 != 0 ) {
	  dose1 = log(dose1)/logFactor;
	} else {
	  dose1 = -99;
	}
      }
      //      if(ist == 107 && ii == 3499259) G4cout << ii << " DOSEInSt " << dose1 << " = " << dosesPerSt[ist] << " stName " << stName << " " << theExcludeStNames[0] << " " << GmGenUtils::AreWordsEquivalent(theExcludeStNames[0],stName) << G4endl; //GDEB
      //     if( ist == 107 ) G4cout << ii << " " << jj << " DOSEInSt " << dose1 << " = " << dosesPerSt[ist] << G4endl; //GDEB
      dose += dose1;
    }
    dose /= ists.size(); // average of doses if voxel belongs to several St
    dosePerSt->at(ii) = dose;
  }
  
  return imageDosePerSt;

}

//---------------------------------------------------------------------------
void DCMGetOrganInfo::DicomException(G4String msg, G4ExceptionSeverity sever )
{
  if( sever == FatalException || sever == FatalErrorInArgument ) {
    PrintHelp();
  }
  G4Exception(theExeName.c_str(),
	      "",
	      sever,
	      msg.c_str());
}


//---------------------------------------------------------------------------
std::pair<G4double,G4double> DCMGetOrganInfo::GetDosimDPercentage(std::map<size_t,mmdd> voxelDoseErrors11, G4String dosimq, size_t id )
{
    if( DicomVerb(infoVerb) ) std::cout << dosimq << " " << id << " GetDosimDPercentage " << " N " << voxelDoseErrors11.size() << std::endl; 

      //  D95=  minimum dose received by >= 95% of each planning target volume
  std::pair<G4double,G4double> dosimQE;
  G4double dosimVal = GmGenUtils::GetValue(dosimq.substr(1,dosimq.length()))/100.;
  if( dosimVal < 0 || dosimVal > 1 ) {
    G4Exception("DCMGetOrganInfo::GetDosimDPercentage",
		"",
		FatalException,
		("Value is negative or bigger than 100 = "+ dosimq).c_str());
  }
  std::cout << id << " GetDosimDPercentage N11 " << voxelDoseErrors11.size() << std::endl; //GDEB
  if( voxelDoseErrors11.size() == 1 ) { // no nRandom values
    mmdd voxelDoseErrors1 = ((*voxelDoseErrors11.begin())).second;
    G4int nVoxelD = voxelDoseErrors1.size();
    G4int NVox = G4int(nVoxelD*dosimVal+1e-6);
    mmdd::iterator ite1 = voxelDoseErrors1.begin();
    //  mmdd::iterator ite1 = voxelDoseErrors1.begin();
    std::advance( ite1, NVox);
    //    G4cout << " GetDosimDPercentage advance " << NVox << " : " << (*ite1).first << "N VOXELS " << voxelDoseErrors1.size() <<  G4endl; //GDEB
    dosimQE = std::pair<G4double,G4double>((*ite1).first,(*ite1).second);
    if( DicomVerb(debugVerb) ){
      std::cout << id << " " << NVox << " GetDosimDPercentage " << std::distance( voxelDoseErrors1.begin(), ite1) << " dosimQ " << (*ite1).first << " +- " <<(*ite1).second << std::endl;
    }
  } else {
    G4double val = 0.;
    G4double valP = 0.;
    G4double valN = 0.;
    G4double valSq = 0.;
    if( DicomVerb(debugVerb) ){
      std::cout << id << " GetDosimDPercentage " << dosimVal << " N " << voxelDoseErrors11.size() << std::endl; 
    }
    std::map<size_t,mmdd>::iterator ite11 = voxelDoseErrors11.begin();
    ite11++; // first is without modifications
    G4int nErr = voxelDoseErrors11.size()-1;
    int ii11 = 0;
    for( ; ite11 != voxelDoseErrors11.end(); ite11++, ii11++ ) {
      mmdd voxelDoseErrors1 = ite11->second;
      G4int nVoxelD = voxelDoseErrors1.size();
      G4int NVox = G4int(nVoxelD*dosimVal+1e-6);
      //      std::cout << id << " GetDosimDPercentage N1 " << nVoxelD << " " << NVox << std::endl; //GDEB
      mmdd::iterator ite1 = voxelDoseErrors1.begin();
      //  mmdd::iterator ite1 = voxelDoseErrors1.begin();
      std::advance( ite1, NVox);
      if( bRandom ) {
	val += (*ite1).first;
	valSq += (*ite1).first*(*ite1).first;	
	/*	int ii = 0;
	for( mmdd::iterator ite = voxelDoseErrors1.begin(); ite != voxelDoseErrors1.end(); ite++, ii++ ) {
	  G4cout << ii11 << " " << ii << " VOXELDOSE " << ite->first << " " << ite->second << G4endl; //GDEB
	  }*/

	if( DicomVerb(debugVerb) ){
	  std::cout << ii11 << " " << id << " " << NVox << " GetDosimDPercentage " << std::distance( voxelDoseErrors1.begin(), ite1) << " dosimQ " << (*ite1).first << " +- " <<(*ite1).second << " -> " << val << " " << valSq << std::endl;
	}
	//	  G4cout << id << ":" << std::distance(voxelDoseErrors11.begin(),ite11) << " GetDosimDPercentage dose " << (*ite1).first << G4endl;
      } else {
	if( std::distance( voxelDoseErrors11.begin(), ite11) == 0 ) {
	  val = (*ite1).first;
	} else if( std::distance( voxelDoseErrors11.begin(), ite11) == 1 ) {
	  valP = (*ite1).first; // DOSE+DOSE_ERROR
	  if( DicomVerb(debugVerb) ){
	    std::cout << id << " GetDosimDPercentage " << std::distance( voxelDoseErrors1.begin(), ite1) << " dosimQ " << (*ite1).first << " +- " <<(*ite1).second << " -> " << valP << std::endl;
	  }
	} else if( std::distance( voxelDoseErrors11.begin(), ite11) == 2 ) {
	  valN = (*ite1).first; // DOSE-DOSE_ERROR
	}
      }
    }
    double valErr;
    if( bRandom ) {
      valErr = sqrt((valSq-val*val/nErr)/(nErr+1));
      val /= nErr;
    } else {
      valErr = fabs(valP-valN)/2;
    }
    if( DicomVerb(debugVerb) ){
      std::cout << id << " GetDosimDPercentage dosimQE " << val << " +- " << valErr << std::endl;
    }
    
    dosimQE = std::pair<G4double,G4double>(val,valErr);

  }

  return dosimQE;
  }

//---------------------------------------------------------------------------
std::pair<G4double,G4double> DCMGetOrganInfo::GetDosimDmincc(std::map<size_t,mmdd> voxelDoseErrors11, G4String dosimq, size_t id)
{
  //  Dmin_Xcc =  minimum dose received by target without counting the X cubic centimeters with smaller dose (sum volume of voxels with smaller dose until V > X cc) " << G4endl
  std::pair<G4double,G4double> dosimQE;
  if( dosimq.substr(dosimq.length()-2,2) != "cc" ) {
    G4Exception("DCMGetOrganInfo::GetDosimDmincc",
		"",
		FatalException,
		("Dmin value has to end by 'cc', while it is "+dosimq).c_str());
  }
  G4double dosimVal = GmGenUtils::GetValue(dosimq.substr(4,dosimq.length()-6));
  dosimVal *= CLHEP::cm3/CLHEP::mm3;
  if( voxelDoseErrors11.size() == 1 ) {
    mmdd voxelDoseErrors1 = ((*voxelDoseErrors11.begin())).second;
    G4int nVoxelD = voxelDoseErrors1.size();
    G4int NVox = G4int(dosimVal/theVoxelVolume_anal-0.00001)+1;
    mmdd::iterator ite1 = voxelDoseErrors1.begin();
    if( NVox > nVoxelD ) {
      G4Exception("DCMGetOrganInfo::GetDosimDmincc",
		  "",
		  JustWarning,
		  (theClassifNames[id]+": Volume requested = "+ dosimq + " is bigger than total structure volume = " + GmGenUtils::ftoa(theVoxelVolume_anal*nVoxelD)).c_str());
    } else {
      std::advance( ite1, NVox);
      G4cout << voxelDoseErrors1.size() << " GetDosimDmincc advance " << NVox << " : " << (*ite1).first << " = " << (*ite1).second <<G4endl; //GDEB
      int ii1 = 0;
      /*      for(mmdd::iterator ite1d = voxelDoseErrors1.begin(); ite1d !=  voxelDoseErrors1.end(); ite1d++, ii1++ ){
	G4cout << " " << ii1 << " DMINCC " << ite1d->first << " " << ite1d->second << " " << G4endl; //GDEB
	}*/
    }
    dosimQE = std::pair<G4double,G4double>((*ite1).first,(*ite1).second);
  } else {
    G4double val = 0.;
    G4double valSq = 0.;
    if( DicomVerb(infoVerb) ){
      std::cout << id << " GetDosimDmincc " << dosimVal << " N " << voxelDoseErrors11.size() << std::endl;
    }
    std::map<size_t,mmdd>::iterator ite11 = voxelDoseErrors11.begin();
    ite11++; // first is without modifications
    G4int nErr = voxelDoseErrors11.size() -1;
    for( ; ite11 != voxelDoseErrors11.end(); ite11++ ) {
      mmdd voxelDoseErrors1 = ite11->second;
      G4int nVoxelD = voxelDoseErrors1.size();
      G4int NVox = G4int(dosimVal/theVoxelVolume_anal-0.00001)+1;
      mmdd::iterator ite1 = voxelDoseErrors1.begin();
      //      G4cerr << " dosimVal=" <<dosimVal << "NVox=" << NVox << " > " << " nVoxelD=" << nVoxelD << G4endl;
      // 	G4cerr << "NVox=" << NVox << " = " << dosimVal << "/" << theVoxelVolume_anal << G4endl; 
      if( NVox > nVoxelD ) {
	G4ExceptionSeverity excSever = JustWarning;
	if( voxelDoseErrors11.size() == 2 ) {
	  excSever = FatalException;
	}
	G4Exception("DCMGetOrganInfo::GetDosimDmincc",
		    "",
		    excSever,
		    (theClassifNames[id]+": Volume requested = "+ dosimq + " is bigger than total structure volume = " + GmGenUtils::ftoa(theVoxelVolume_anal*nVoxelD)).c_str());
	nErr -= 1; 
      } else {
	std::advance( ite1, NVox);
	//  G4cout << voxelDoseErrors1.size() << " GetDosimDmincc advance " << NVox << " : " << dose  << " TOTAL VOL " << theVoxelVolume_anal*nVoxelD << " mm3 " <<G4endl; //GDEB
	val += (*ite1).first;
	valSq += (*ite1).first*(*ite1).first;
	G4cout << id << ":" << std::distance(voxelDoseErrors11.begin(),ite11) << " GetDosimDmincc dose " << (*ite1).first << G4endl;
      }
    }
    double valErr = sqrt((valSq-val*val/nErr)/(nErr+1));
    val /= nErr;

    dosimQE = std::pair<G4double,G4double>(val,valErr);
  }
  return dosimQE;
}

//---------------------------------------------------------------------------
std::pair<G4double,G4double> DCMGetOrganInfo::GetDosimDmaxcc(std::map<size_t,mmdd> voxelDoseErrors11, G4String dosimq, size_t id)
{
  //  Dmax_Xcc =  maximum dose received by target without counting the X cubic centimeters of bigger dose (sum volume of voxels with bigger dose until V > X cc) " << G4endl
  std::pair<G4double,G4double> dosimQE;
  if( dosimq.substr(dosimq.length()-2,2) != "cc" ) {
    G4Exception("DCMGetOrganInfo::GetDosimDmaxcc",
		"",
		FatalException,
		("Dmax value has to end by 'cc', while it is "+dosimq).c_str());
  }
  G4double dosimVal = GmGenUtils::GetValue(dosimq.substr(4,dosimq.length()-6));
  dosimVal *= CLHEP::cm3/CLHEP::mm3;
  G4int NVox = G4int(dosimVal/theVoxelVolume_anal-0.00001)+1;
  if( voxelDoseErrors11.size() == 1 ) {
    mmdd voxelDoseErrors1 = ((*voxelDoseErrors11.begin())).second;
    G4int nVoxelD = voxelDoseErrors1.size();
    mmdd::reverse_iterator ite1 = voxelDoseErrors1.rbegin();
    if( NVox > nVoxelD ) {
      G4cerr << " NVoxREQ " << NVox << " = " << dosimVal << " / " << theVoxelVolume_anal << G4endl; 
      G4Exception("DCMGetOrganInfo::GetDosimDmaxcc",
		  "",
		  JustWarning,
		  (theClassifNames[id]+": Volume requested = "+ dosimq + " is bigger than total structure volume = " + GmGenUtils::ftoa(theVoxelVolume_anal*nVoxelD)).c_str());
    } else {
      std::advance( ite1, NVox);
    }
     dosimQE = std::pair<G4double,G4double>((*ite1).first,(*ite1).second);
     //     G4cout << voxelDoseErrors1.size() << " GetDosimDmaxcc advance " << NVox << " : " << (*ite1).first << " TOTAL VOL " << theVoxelVolume_anal*nVoxelD << " mm3 " <<G4endl; //GDEB
  } else {
    G4double val = 0.;
    G4double valSq = 0.;
    if( DicomVerb(infoVerb) ){
      std::cout << id << " GetDosimDmaxcc" << dosimVal << " N " << voxelDoseErrors11.size() << std::endl;
    }
    std::map<size_t,mmdd>::iterator ite11 = voxelDoseErrors11.begin();
    ite11++; // first is without modifications
    G4int nErr = voxelDoseErrors11.size() - 1;
    for( ; ite11 != voxelDoseErrors11.end(); ite11++ ) {
      mmdd voxelDoseErrors1 = ite11->second;
      G4int nVoxelD = voxelDoseErrors1.size();
      mmdd::reverse_iterator ite1 = voxelDoseErrors1.rbegin();
      if( NVox > nVoxelD ) {
	G4cerr << " NVoxREQ " << NVox << " = " << dosimVal << " / " << theVoxelVolume_anal << G4endl; 
	G4ExceptionSeverity excSever = JustWarning;
	if( voxelDoseErrors11.size() == 2 ) {
	  excSever = FatalException;
	}
	G4Exception("DCMGetOrganInfo::GetDosimDmaxcc",
		    "",
		    excSever,
		    (theClassifNames[id]+": Volume requested = "+ dosimq + " is bigger than total structure volume = " + GmGenUtils::ftoa(theVoxelVolume_anal*nVoxelD)).c_str());
	nErr -= 1; 
      } else {
	std::advance( ite1, NVox);
	val += (*ite1).first;
	valSq += (*ite1).first*(*ite1).first;
	G4cout << id << ":" << std::distance(voxelDoseErrors11.begin(),ite11) << " GetDosimDmaxcc dose " << (*ite1).first << G4endl;
      }
      //-  G4cout << voxelDoseErrors1.size() << " GetDosimDmaxcc advance " << NVox << " : " << dose << " TOTAL VOL " << theVoxelVolume_anal*nVoxelD << " mm3 " <<G4endl; //GDEB
    }
    double valErr = sqrt((valSq-val*val/nErr)/(nErr+1));
    val /= nErr;

    dosimQE = std::pair<G4double,G4double>(val,valErr);
  }
  
  return dosimQE;
}

//---------------------------------------------------------------------------
std::pair<G4double,G4double> DCMGetOrganInfo::GetDosimDmean(std::map<size_t,mmdd> voxelDoseErrors11, size_t id)
{
  //  Dmax_Xcc =  maximum dose received by target without counting the X cubic centimeters of bigger dose (sum volume of voxels with bigger dose until V > X cc) " << G4endl
  std::pair<G4double,G4double> dosimQE;
  if( voxelDoseErrors11.size() == 1 ) {
    mmdd voxelDoseErrors1 = ((*voxelDoseErrors11.begin())).second;
    G4double dmean = 0;
    G4double dmeanErr = 0;
    G4int nVoxelD = voxelDoseErrors1.size();
    for( mmdd::const_iterator ite1 = voxelDoseErrors1.begin(); ite1 != voxelDoseErrors1.end(); ite1++ ) {
      dmean += (*ite1).first;
      dmeanErr += (*ite1).second;
    }
    dmean /= nVoxelD;
    dmeanErr /= nVoxelD;
    dosimQE = std::pair<G4double,G4double>(dmean,dmeanErr);
     //     G4cout << voxelDoseErrors1.size() << " GetDosimDmaxcc advance " << NVox << " : " << (*ite1).first << " TOTAL VOL " << theVoxelVolume_anal*nVoxelD << " mm3 " <<G4endl; //GDEB
  } else {
    G4double val = 0.;
    G4double valSq = 0.;
    if( DicomVerb(infoVerb) ){
      std::cout << id << " GetDosimDmean N " << voxelDoseErrors11.size() << std::endl;
    }
    std::map<size_t,mmdd>::iterator ite11 = voxelDoseErrors11.begin();
    ite11++; // first is without modifications
    G4int nErr = voxelDoseErrors11.size() - 1;
    for( ; ite11 != voxelDoseErrors11.end(); ite11++ ) {
      mmdd voxelDoseErrors1 = ite11->second;
      G4int nVoxelD = voxelDoseErrors1.size();
      G4double dmean = 0;
      for( mmdd::const_iterator ite1 = voxelDoseErrors1.begin(); ite1 != voxelDoseErrors1.end(); ite1++ ) {
	dmean += (*ite1).first;
      }
      dmean /= nVoxelD;
      val += dmean;
      valSq += dmean*dmean;
      G4cout << id << ":" << std::distance(voxelDoseErrors11.begin(),ite11) << " GetDosimDmean " << dmean << G4endl;
      //-  G4cout << voxelDoseErrors1.size() << " GetDosimDmaxcc advance " << NVox << " : " << dose << " TOTAL VOL " << theVoxelVolume_anal*nVoxelD << " mm3 " <<G4endl; //GDEB
    }
    double valErr = sqrt((valSq-val*val/nErr)/(nErr+1));
    val /= nErr;

    dosimQE = std::pair<G4double,G4double>(val,valErr);
  }
  
  return dosimQE;

}


//---------------------------------------------------------------------------
std::pair<G4double,G4double> DCMGetOrganInfo::GetDosimVolume(std::map<size_t,mmdd> voxelDoseErrors11, G4String dosimq, size_t id)
{
  // V95 = volume that receives >= 95% of prescribed dose 
  std::pair<G4double,G4double> dosimQE;
  if( thePrescribedDose == -1 ) {
    G4Exception("DCMGetOrganInfo::GetDosimVolume",
		"",
		FatalException,
		"No prescribed dose defined, please use the parameter : -prescribedDose VALUE");
  }
  
  G4double dosimVal = GmGenUtils::GetValue(dosimq.substr(1,dosimq.length()))/100.;
  G4double dose = dosimVal * thePrescribedDose;
  G4cout << "DCMGetOrganInfo::GetDosimVolume NVoxelDoseErrors11 " << voxelDoseErrors11.size() << G4endl; //GDEB
  if( voxelDoseErrors11.size() == 1 ) {
    mmdd voxelDoseErrors1 = ((*voxelDoseErrors11.begin())).second;
    mmdd::iterator ite1 = voxelDoseErrors1.upper_bound(dose); // bigger than
    G4int NVox;
    if( ite1 == voxelDoseErrors1.end() ) {
      G4Exception("DCMGetOrganInfo::GetDosimVolume",
		  "",
		  JustWarning,
		  (theClassifNames[id]+": Dose requested = "+ GmGenUtils::ftoa(dose) + " is bigger than dose in any voxel = " + GmGenUtils::ftoa((*(voxelDoseErrors1.rbegin())).first)).c_str());
      NVox = voxelDoseErrors1.size();
    } else {
      NVox = (std::distance(ite1,voxelDoseErrors1.end()));
    }
    G4double volume = float(NVox)/voxelDoseErrors1.size();
    G4cout << dosimVal << " " << dose << " DOSIMQ V " << volume << "= " << NVox << "/" << voxelDoseErrors1.size() << G4endl; //GDEB
    
    G4double doseErr = (*ite1).second;
    ite1 = voxelDoseErrors1.upper_bound(dose+doseErr); // bigger than
    if( ite1 == voxelDoseErrors1.end() ) {
      G4Exception("DCMGetOrganInfo::GetDosimVolume",
		  "",
		  JustWarning,
		  (theClassifNames[id]+": Dose requested = "+ GmGenUtils::ftoa(dose) + " is bigger than dose in any voxel = " + GmGenUtils::ftoa((*(voxelDoseErrors1.rbegin())).first)).c_str());
      NVox = voxelDoseErrors1.size();
    } else {
      NVox = (std::distance(ite1,voxelDoseErrors1.end()));
    }
    G4double volumep = float(NVox)/voxelDoseErrors1.size();
    ite1 = voxelDoseErrors1.upper_bound(dose-doseErr); // bigger than
    if( ite1 == voxelDoseErrors1.end() ) {
      G4Exception("DCMGetOrganInfo::GetDosimVolume",
		  "",
		  JustWarning,
		  (theClassifNames[id]+": Dose requested = "+ GmGenUtils::ftoa(dose) + " is bigger than dose in any voxel = " + GmGenUtils::ftoa((*(voxelDoseErrors1.rbegin())).first)).c_str());
      NVox = voxelDoseErrors1.size();
    } else {
      NVox = (std::distance(ite1,voxelDoseErrors1.end()));
    }
    G4double volumem = float(NVox)/voxelDoseErrors1.size();
    
    /*    //--- Get volume if dose = dose+error
    std::set<G4double> vdee; 
    for( mmdd::iterator ite = voxelDoseErrors1.begin(); ite != voxelDoseErrors1.end(); ite++ ) {
      vdee.insert( (*ite).first+(*ite).second );    
    }
    std::set<G4double>::iterator itee = vdee.upper_bound(dose);
    NVox = std::distance(vdee.begin(),itee);
    G4double volumep = float(NVox)/voxelDoseErrors1.size();
    
    //--- Get volume if dose = dose-error
    vdee.clear();  
    for(  mmdd::iterator ite = voxelDoseErrors1.begin(); ite != voxelDoseErrors1.end(); ite++ ) {
      vdee.insert( (*ite).first+(*ite).second );    
    }
    itee = vdee.upper_bound(dose);
    NVox = std::distance(vdee.begin(),itee);
    G4double volumem = float(NVox)/voxelDoseErrors1.size();
    */
    
    G4double volErr = max(volumem-volume,volume-volumep);
    
    dosimQE = std::pair<G4double,G4double>(volume*100,volErr*100); // value in %

  } else {
    G4double val = 0.;
    G4double valSq = 0.;
    if( DicomVerb(infoVerb) ){
      std::cout << id << " GetDosimDmaxcc" << dosimVal << " N " << voxelDoseErrors11.size() << std::endl; //GDEB
    }
    std::map<size_t,mmdd>::iterator ite11 = voxelDoseErrors11.begin();
    ite11++; // first is without modifications
    G4int nErr = voxelDoseErrors11.size() - 1;
    int ii11 = 0; //GDEB
    for( ; ite11 != voxelDoseErrors11.end(); ite11++, ii11++ ) {
      mmdd voxelDoseErrors1 = ite11->second;
      /*      int ii = 0;
      for( mmdd::iterator ite = voxelDoseErrors1.begin(); ite != voxelDoseErrors1.end(); ite++, ii++ ) {
	G4cout << ii11 << " " << ii << " VOXELDOSE " << ite->first << " " << ite->second << G4endl; //GDEB
	}*/
      mmdd::iterator ite1 = voxelDoseErrors1.upper_bound(dose); // bigger than
      G4int NVox;
      if( ite1 == voxelDoseErrors1.end() ) {
	G4Exception("DCMGetOrganInfo::GetDosimVolume",
		    "",
		    JustWarning,
		    (theClassifNames[id]+": Dose requested = "+ GmGenUtils::ftoa(dose) + " is bigger than dose in any voxel = " + GmGenUtils::ftoa((*(voxelDoseErrors1.rbegin())).first)).c_str());
	NVox = voxelDoseErrors1.size();
      } else {
	NVox = (std::distance(ite1,voxelDoseErrors1.end()));
      }
      G4double volume = float(NVox)/voxelDoseErrors1.size();
      G4cout << dosimVal << " " << dose << " DOSIMQ V " << volume << "= " << NVox << "/" << voxelDoseErrors1.size() << G4endl;

      val += volume;
      valSq += volume*volume;
      
    }
    double valErr = sqrt((valSq-val*val/nErr)/(nErr+1));
    val /= nErr;
    dosimQE = std::pair<G4double,G4double>(val*100.,valErr*100.); //value in %
  }
  
  return dosimQE;
}

//-------------------------------------------------------------------
void DCMGetOrganInfo::PrintHelp()
{
  ((DicomParameterMgr*)(DicomParameterMgr::GetInstance()))->PrintHelpInputFiles();
  G4cout << " -fRTDOSE     dose image file in DICOM RTDOSE format" << G4endl
	 << " -fSqDose     dose image file in GAMOS SqDose format " << G4endl
	 << " -fG4dcmDose  dose image file in Geant4 g4dcm format" << G4endl
	 << " -fNM         NM image file in DICOM PT format" << G4endl
	 << " -fG4dcmNM    NM image file in Geant4 g4dcm format" << G4endl
    	 << " -fG4dcmCT    CT image file in G4 DICOM format" << G4endl
	 << " -fOut        Output file " << G4endl
	 << " -bDepoEnergy   Calculate deposited energy per event (1) or S-value (0). Default is S-value " << G4endl
	 << " -bPerMate   Calculate for each material (1) or for each structure (0). Default is for each structure " << G4endl
    //	 << " -multFactor  Factor to be multiplied by the dose/energy deposition to change the unit" << G4endl
	 << " -prescribedDose   dose prescribed, to be used by dosimetric quantity Vnn" << G4endl
	 << " -multDoseByNumber   number of events to calculate the dose. Default is 1 " << G4endl
	 << " -dosimQ/-dosimQuantity   dosimetric quantity to be calculated for each structure/materials " << G4endl
	 << "       Dnn =  minimum dose received by nn% volume of the target volumes " << G4endl
	 << "       Dmin_Xcc =  minimum dose received by target without counting the X cubic centimeters with smaller dose (sum volume of voxels with smaller dose until V > X cc) " << G4endl
	 << "       Dmax_Xcc =  maximum dose received by target without counting the X cubic centimeters of bigger dose (sum volume of voxels with bigger dose until V > X cc) " << G4endl
	 << "       Vnn = volume that received nn% or more of the prescribed dose (-prescribedDose should be given)"
	 << "       Dmean = average dose in structure" << G4endl
	 << " -prescribedDose   Prescribed dose to be used for dosimetric quantity Vnn " << G4endl
	 << " -minHistoDose    Minimum value in X axis for Dose and Dose-Volume Histograms. Default is 0" << G4endl
    	 << " -maxHistoDose    Maximum value in X axis for Dose and Dose-Volume Histograms. Default is maximum dose in a voxel" << G4endl
    	 << " -bLogOrganInfo   Use logarithm scale in X axis for Dose and Dose-Volume Histograms. Default is linear scale" << G4endl
	 << " -palette NUMBER   choose integer code of color palette (for codes see https://root.cern.ch/doc/master/classTColor.html) " << G4endl
	 << " -paletteMin VALUE   set minimum of palette" << G4endl
	 << " -paletteMax VALUE   set maximum of palette" << G4endl
	 << " -bDrawImageDoseInSt   Draw an image where each voxel has the value of the dose in the structure it belongs to (average if it belongs to several structures. Default is 0" << G4endl
	 << " -bHistos     Fill Dose Histograms and Dose-Volume Histograms and save them in a .root file" << G4endl
	 << " -histosLogN VALUE   Values of imageDoseInSet are converted by logarithm base N" << G4endl
	 << " -nRandomizeError VALUE   Calculate statistical fluctuations of dosimetric quantities changing each dose value by the value sampling a Gaussian with sigma equal to the dose error value (for sqdose); try VALUE times and get the average " << G4endl
	 << " -nErrorSigmas VALUE      Use VALUE*dose_error_value for the nRandomizeError calculations" << G4endl
	 << " -plusError VALUE         Calculate dosimetric quantities adding to the dose value the dose error value * VALUE (for sqdose)" << G4endl
	 << " -minusError VALUE        Calculate dosimetric quantities subtracting to the dose value the dose error value * VALUE (for sqdose)" << G4endl
	 << " -errorImageRelErr VALUE  Calculate errors by building an error image with values of image multiplied by VALUE " << G4endl
	 << " -verb        sets the verbosity: test/debug/info/warning/error/silent" << G4endl
	 << " -h           prints the set of arguments " << G4endl
	 << " -help        prints the set of arguments " << G4endl;
}
 
/*t	  argvstr == "-bVolumeMass" ||
	  //	  argvstr == "-multFactor" ||
	  argvstr == "-minMaxError" ||
	  argvstr == "-nErrorSigmas" ||
*/


//-------------------------------------------------------------------
DicomVImage* DCMGetOrganInfo::BuildAnalyseErrorImage( DicomVImage* image, G4double relErr )
{
  G4double maxValue = image->GetMaxValue();
  DicomVImage* errImage = new DicomVImage(*image);
  //  if( DicomVerb(infoVerb) ) G4cout << " Dicom2DOperAddError::Operate " << imageErr->GetData(1000000) << G4endl; //GDEB
  size_t nVox = errImage->GetNoVoxels();
  std::vector<G4double>* data = image->GetData(); 
  G4double* dataP = &(data->at(0));
  std::vector<G4double>* dataErr = new std::vector<G4double>(nVox);
  errImage->SetData(dataErr);
  G4double* dataErrP = &(dataErr->at(0));
  for( size_t ii = 0; ii < nVox; ii++, dataP++, dataErrP++ ) {
    G4double relErr1 = 0.;
    if( *dataP != 0. ) {
      relErr1 = *dataP * relErr * sqrt(maxValue/(*dataP)); // proportional to dose value
    }
    G4double newData = relErr1;
    (*dataErrP) = newData;
    //    if( ii%100000 == 0 ) G4cout << ii << " BuildAnalyseErrorImage = " << " " << (*dataErrP) << " = " << (*dataP) << " , " << relErr1 << G4endl; //GDEB
  }

  return errImage;

}
