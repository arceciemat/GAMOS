//#define PAPER
#include <fstream>
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosRunManager/include/GmRunManager.hh"
#include "ROOTUtilities/PlotData/DrawHisto.C"
#include "ROOTUtilities/PlotData/PlotUtils.C"
#include "ROOTUtilities/PlotData/CompareStats.C"
#include "getGammaIndex1CD.C"
#include "DoseSet.hh"
#include "DoserData.hh"
#include "FileData.hh"

static G4bool bInvertX;

void PrintHelp();
void ReadDrawingData( G4String fileName );
G4double GetDrawingData( G4String dataName, G4double def = 0.);
G4String GetDrawingDataString( G4String dataName, G4String def= "");
G4String GetDrawingDataMulti( G4String dataName, G4int ipd );
void ReadCompareFile( G4String fileName );
void PlotDosers( DoseSet* doseSet, G4bool bLogX );
DoserData* FindDoserData( G4String fileName , G4String doserName );
void SetUserHistoLimits();

std::map<G4String,G4String> theDrawingData;
std::vector<FileData*> theFileData;
typedef  std::map<G4String,std::vector<G4String> > msvs;
msvs theDrawingDataMulti;
G4double theHXMin, theHXMax;
G4double theHYMin, theHYMax;
std::vector<DoseSet*> theDoseSets; 
std::set<pss> theFiles; // fileName - fileType
WHLType xWHLType;
WHLType yWHLType;
G4bool bHistoLabel = true;

#include "ReadDoses.icc"

//-------------------------------------------------------------------
int main(int argc,char** argv) 
{
  theHXMin = -DBL_MAX;
  theHXMax = -DBL_MAX;
  theHYMin = -DBL_MAX;
  theHYMax = -DBL_MAX;
  Definitions::bLogX = false;
  Definitions::bLogY = false;

  G4String theCompareFileName = "doseCompare.lis";
  G4String theDrawingDataFileName = "drawingData.lis";

  if( argc == 2 ) {
    if( G4String(argv[1]) == "-h" || G4String(argv[1]) == "-help" ) {
      PrintHelp();
      return 0;
    } else {
      theCompareFileName = argv[1];
    }
  } else {
    if(argc%2 != 1) { 
      G4Exception("compareDoses",
		  "Error",
		  FatalErrorInArgument,
		  "WRONG NUMBER OF ARGUMENTS: THEY MUST BE -XX1 VAL_XX1 -XX2 VAL_XX2 ... ");
    }
    for( G4int ii = 1; ii < argc; ii+=2 ){
      G4String argvstr = argv[ii];
      if( argvstr == "-f" ) {
	theCompareFileName = argv[ii+1];
      } else if( argvstr == "-fDrawingData" ) {
	theDrawingDataFileName = argv[ii+1];
      } else if( argvstr == "-hXLog" ) {
	Definitions::bLogX = GmGenUtils::GetBool(argv[ii+1]);
      } else if( argvstr == "-hYLog" ) {
	Definitions::bLogY = GmGenUtils::GetBool(argv[ii+1]);
      } else if( argvstr == "-histoLabel" ) {
	bHistoLabel = GmGenUtils::GetBool(argv[ii+1]);
      } else if( argvstr == "-bInvertX" ) {
	bInvertX = GmGenUtils::GetBool(argv[ii+1]);
      } else if( argvstr == "-verb" ) {
	G4String verbstr = G4String(argv[ii+1]);
 	if(verbstr == "test" ) {
	  DoseData::verbosity = 3;
	  verbose = 3;
	} else if(verbstr == "debug" ) {
	  DoseData::verbosity = 2;
	  verbose = 2;
	} else if(verbstr == "info" ) {
	  DoseData::verbosity = 1;	
	  verbose = 1;
	} else if(verbstr == "warning" ) {
	  DoseData::verbosity = 0;
	  verbose = 0;
	} else if(verbstr == "error" ) {
	  DoseData::verbosity = -1;
	  verbose = -1;
	} else {
	  G4cerr << " VERBOSITY " << argv[ii+1]  << G4endl;
	  G4Exception("compareDoses",
		  "Error",
		  FatalErrorInArgument,
		      "verbosity wrong");
	}
	verbstr = "ShieldingVerbosity " + verbstr;
	GmRunManager* runManager = new GmRunManager();
	runManager->SelectVerbosity(verbstr);
	if( G4String(argv[ii+1]) == "info" ) {
	  verbose = 2;       
	}else if( G4String(argv[ii+1]) == "debug" ) {
	  verbose = 3;
	}
      } else if( argvstr == "-hXMin" ) {
	theHXMin = GmGenUtils::GetValue(argv[ii+1]);
      } else if( argvstr == "-hXMax" ) {
	theHXMax = GmGenUtils::GetValue(argv[ii+1]);
      } else if( argvstr == "-hYMin" ) {
	theHYMin = GmGenUtils::GetValue(argv[ii+1]);
      } else if( argvstr == "-hYMax" ) {
	theHYMax = GmGenUtils::GetValue(argv[ii+1]);
      } else if( argvstr == "-indexValType" ) {
	G4String idxstr = G4String(argv[ii+1]);
 	if(idxstr == "upper" || idxstr == "UPPER" ) {
	  DoseData::theIndexValType = SIVT_UPPER;
	} else if(idxstr == "middle" || idxstr == "MIDDLE" ) {
	  DoseData::theIndexValType = SIVT_MIDDLE;
	} else if(idxstr == "lower" || idxstr == "LOWER" ) {
	  DoseData::theIndexValType = SIVT_LOWER;
	} else {
	  G4cerr << " INDEX VAL TYPE " << idxstr << G4endl;
	  G4Exception("compareDoses",
		      "Error",
		      FatalErrorInArgument,
		      (G4String("Wrong index val type, it can be upper/UPPER, middle/MIDDLE or lower/LOWER, while it is") + idxstr).c_str());
	}
      } else {
	PrintHelp();
	G4Exception("compareDoses",
		    "Error",
		    FatalErrorInArgument,
		    (G4String("ARGUMENT: ") + argvstr).c_str());
      }
    }
  }

  if( theDrawingDataFileName != "" ) ReadDrawingData( theDrawingDataFileName );

  if( theCompareFileName == "" ) {
    G4Exception("compareDoses",
		"Error",
		FatalException,
		"You have  not asked for a file, please use -f"); 
  }

  if( DoseData::verbosity >= 1) G4cout << "@@@@@@@ READING COMPARE META FILE: " << theCompareFileName << G4endl;
  ReadCompareFile( theCompareFileName );

  std::set<pss>::const_iterator itef;
  for( itef = theFiles.begin(); itef != theFiles.end(); itef++ ) {
    if( DoseData::verbosity >= 1) G4cout << "@@@@@@@ READING FILE: " << (*itef).first << " of type " << (*itef).second << G4endl;
    ReadDoses( (*itef).first, (*itef).second );
  }

  std::vector<DoseSet*>::const_iterator itess;
  G4bool bLogX = Definitions::bLogX; // use the same for all dosers, even if it is change because data is negative. If this happen the drawing will be done linerally, but the GetIndexVal not, because else comparison would not be possible
  for( itess = theDoseSets.begin(); itess != theDoseSets.end(); itess++ )  {
    if( DoseData::verbosity >= 1) G4cout << "@@@@@@@ PLOT DOSERS: " << (*itess)->GetHisFileName() << G4endl;
    //    G4cout << " CALL PLOT DATA " << bLogX << G4endl;//GDEB
    PlotDosers(*itess,bLogX);
  }

  return 0;
}


//-----------------------------------------------------------------------------
void ReadCompareFile( G4String fileName )
{
  std::vector<G4String> wl;
  GmFileIn fin = GmFileIn::GetInstance(fileName);
  DoseSet* doseSet = 0;
  for( G4int ii = 0;; ii++) {
    if( ! fin.GetWordsInLine(wl) ) break;
    if( wl[0] == "COMPARE" ) {
      doseSet = new DoseSet(wl, Definitions::bLogX, Definitions::bLogY);
      theDoseSets.push_back(doseSet);
    } else {
      if( doseSet == 0 ) {
	G4Exception("ReadCompareFile",
		    "",
		    FatalException,
		    "First line in file must start with ' COMPARE '");
      }
      G4String fName;
      G4String doseName;
      G4String fType;
      G4double multFactor = 1.;
      G4double dispFactor = 0.;
      G4int iColor = -1;
      fType = "ROOT";
      G4double dispYFactor = 0.;
      switch(wl.size() ){
       case 7:
	dispYFactor = GmGenUtils::GetValue( wl[6] );
#ifndef WIN32
	[[fallthrough]];
#endif
       case 6:
	iColor = GmGenUtils::GetInteger(wl[5]);
	//	std::cout << wl[1] << " READ COLOR " << iColor << " " << wl[5] << std::endl; //GDEB
#ifndef WIN32
	[[fallthrough]];
#endif
      case 5:
	dispFactor = GmGenUtils::GetValue( wl[4] );
#ifndef WIN32
	[[fallthrough]];
#endif
      case 4:
	multFactor = GmGenUtils::GetValue( wl[3] );
#ifndef WIN32
	[[fallthrough]];
#endif
      case 3:
	fType = wl[2];
#ifndef WIN32
	[[fallthrough]];
#endif
      case 2:
	fName = wl[0];
	doseName = wl[1];
	break;
      default : 
	G4cerr << "READING LINE";
	for( size_t ii2 = 0; ii2 < wl.size(); ii2++ ) {
	  G4cerr << " " << wl[ii2];
	}
	G4cerr << G4endl;
	G4Exception("ReadCompareFile",
		    "",
		    FatalException,
		    (G4String("Lines line must have less than 7 words, while it has") + GmGenUtils::itoa(wl.size())).c_str());
      }
      theFiles.insert(pss(fName,fType));
      doseSet->AddDoser(fName,doseName,multFactor,dispFactor, iColor, dispYFactor);

    }
  }
}

//-----------------------------------------------------------------------------
void PlotDosers( DoseSet* doseSet, G4bool bLogX  ) 
{
  //--- CHANGE DRAWING VARIABLES
  Definitions::markerSize = GetDrawingData("markerSize",1);
  Definitions::markerTextDispX = GetDrawingData("markerTextDispX",15);
  Definitions::markerTextDispY = GetDrawingData("markerTextDispY",0.5);
  Definitions::markerTextSize = GetDrawingData("markerTextSize",0.5);
  Definitions::lineJoinMarker = GetDrawingData("lineJoinMarker",4);
  Definitions::yAxisTitle = theDrawingData["yAxisTitle"];
  Definitions::xAxisTitleOffset = GetDrawingData("xAxisTitleOffset",0.7);
  Definitions::yAxisTitleOffset = GetDrawingData("yAxisTitleOffset",1.);
  Definitions::xAxisTitleSize = GetDrawingData("xAxisTitleSize",0.05);
  Definitions::yAxisTitleSize = GetDrawingData("yAxisTitleSize",0.05);
  Definitions::xAxisLabelSize = GetDrawingData("xAxisLabelSize",0.04);
  Definitions::yAxisLabelSize = GetDrawingData("yAxisLabelSize",0.04);
  Definitions::xAxisLabelOffset = GetDrawingData("xAxisLabelOffset",0.);
  Definitions::yAxisLabelOffset = GetDrawingData("yAxisLabelOffset",0.);
  Definitions::xAxisTitle = GetDrawingDataString("xAxisTitle","mm");
  Definitions::yAxisTitle = GetDrawingDataString("yAxisTitle","Dose");
  Definitions::padLeftMargin = GetDrawingData("padLeftMargin",1);
  Definitions::padRightMargin = GetDrawingData("padRightMargin",1);
  Definitions::xHistoLabel = GetDrawingData("xHistoLabel",40.);
  Definitions::yHistoLabel = GetDrawingData("yHistoLabel",270.);

  //----- DoseSet IS THE SET OF DOSERS TO COMPARE, WHICH START BY LINE
  // COMPARE 0 profY10x10.dALL.dataCUN
  //--- Init histo limits
  Definitions::canvasSizeX = GetDrawingData("canvasSizeX",1000);
  Definitions::canvasSizeY = GetDrawingData("canvasSizeY",1000);
  Definitions::TOTAL_WIDTH = 540*Definitions::canvasSizeX/500;  //?
  Definitions::TOTAL_HEIGTH = 390*Definitions::canvasSizeY/500;  //?
  theCanvas->SetCanvasSize(Definitions::canvasSizeX, Definitions::canvasSizeY);
  Definitions::bLogX = doseSet->IsLogX();
  Definitions::bLogY = doseSet->IsLogY();

  //----- DoseInfo IS THE DOSE INFORMATION OF EACH DOSER:
  // data//ELEKTA_.prof10x10.data.txt ProfileY10x10.d5.data TEXT 1. -0.310464 0
  // prod/dose_analyseSqdose..standard.20.13..0..0..0..8309_ALL.root ProfX_Z=20cm ROOT  0. 0
  std::vector<DoseInfo> fDoseInfos = doseSet->GetDoseInfos();
  G4bool bNorm = doseSet->IsNormalized();
  
  vpsmdpdd plotDosers;
  std::map<G4String,G4int> theIdxColors;

  //----- TRANSFORM EACH DoseInfo TO PLOTTING FORMAT :
  // msmdpdd plotDosers[(*itess).fileName+"_"+(*itess).doseName] = doserValues; 
  // mdpdd doserValues : X_val std::pair<Y_val,Y_err>
  std::vector<DoseInfo>::const_iterator itess;
  G4bool bEmpty = true;
  for( itess = fDoseInfos.begin(); itess != fDoseInfos.end(); itess++ ) {
    DoserData* doserData = FindDoserData( (*itess).fileName, (*itess).doseName );
    if( doserData->GetNDoses() != 0 ) bEmpty = false;
    if( DoseData::verbosity >= 1) G4cout << "@@@@@ PLOT DOSE INFO: " << (*itess).fileName << " " << (*itess).doseName << " N_DOSES " << doserData->GetNDoses() << G4endl;
    //-    G4cout << " DOSER DATA " << doserData << G4endl;
    if( doserData ) {
      mdpdd* doserValues = doserData->GetPlotData(bLogX, (*itess).multFactor, (*itess).dispFactor, (*itess).dispYFactor );
      if( Definitions::bLogX ) {
	mdpdd::const_iterator ite;
	for( ite = doserValues->begin(); ite != doserValues->end(); ite++ ) {
	  if( (*ite).first <= 0. ) {
	    G4Exception("PlotDosers",
			"",
			JustWarning,
			(G4String("Asking for logarithmic X axis while values are <= 0 : ") + GmGenUtils::ftoa((*ite).first)).c_str());
	    //-	    doserValues = doserData->GetPlotData(0); the drawing will be done linerally, but the GetIndexVal not, because else comparison would not be possible
	    break;
	  }
	}
      }
      theIdxColors[(*itess).fileName+"_"+(*itess).doseName] = (*itess).iColor;
      //      G4cout << " FILL idxColors " << (*itess).fileName+"_"+(*itess).doseName << " = " << (*itess).iColor << G4endl; //GDEB
      plotDosers.push_back(psmdpdd((*itess).fileName+"_"+(*itess).doseName, doserValues));
      if( DoseData::verbosity >= 2) G4cout << "@@ FILLED doserValues (mdpdd) " << (*itess).fileName+"_"+(*itess).doseName << " N " << doserValues->size() << G4endl;

    }
  }

  //------ DO NOT CONTINUE IF ALL DOSERS IN THIS SET ARE EMPTY
  if( bEmpty ) {
    G4cerr << " compareDoses: all doses are empty, exiting " << G4endl;
    return;
  }

  vpsmdpdd::const_iterator itecs; // loop to dosers in file
  mdpdd::iterator itec; // loop to doses in doser

  //----- NORMALIZE DOSERS IF REQUESTED (INTEGRAL OF EACH DOSER -> 1)
  if( bNorm ) {
    for( itecs = plotDosers.begin(); itecs != plotDosers.end(); itecs++ ) {
      G4double sumDose = 0.;
      mdpdd* data1 = (*itecs).second;
      for( itec = data1->begin(); itec != data1->end(); itec++ ) {
	sumDose += (*itec).second.first;
      }
      for( itec = data1->begin(); itec != data1->end(); itec++ ) {
	(*itec).second.first /= sumDose;
	(*itec).second.second /= sumDose;
      }
      if( DoseData::verbosity >= 1) G4cout << "@@ data NORMALIZED " << (*itecs).first << " sumDose= " << sumDose << G4endl;
    }
  }
 
  //----- LOOP TO plotDosers TO PLOT ONE HISTOGRAM PER DOSER
  G4int ipd = 0;
  G4double xInit = 30*theCanvas->GetWw()/500;
  G4double yInit = 378*theCanvas->GetWh()/500;
  G4String hisFileName = doseSet->GetHisFileName();
  TH1F* his = 0;

  //----- Data SETS THE HISTO LIMITS, NOT MC = 
  //  Definitions::x/yDataMin/Max. from doser min/max
  //--- FIND IF THERE IS DATA
  G4bool bThereIsData = false;
  for( itecs = plotDosers.begin(); itecs != plotDosers.end(); itecs++) {
    if( (*itecs).first.find("data") != std::string::npos ) {
      bThereIsData = true;
      break;
    }
  }
  if( DoseData::verbosity >= 1) G4cout << "@@ Data sets histo limits? " << bThereIsData << G4endl;
  //--- IF THERE IS DATA ONLY USE DATA, ELSE USE ALL
  G4int nPlots = plotDosers.size();
  for( itecs = plotDosers.begin(); itecs != plotDosers.end(); itecs++) {
    mdpdd* data1 = (*itecs).second;
    G4bool bIsFirst = (itecs == plotDosers.begin());
    // Data SETS THE HISTO LIMITS, NOT MC
    if( !bThereIsData || (*itecs).first.find("data") != std::string::npos ) SetHistoLimits( data1, bIsFirst ); // SetHistoLimits if this one is data
  }

  //----- SET HISTO LIMITS IF SET BY USER
  xWHLType = WHL_MinMax;
  yWHLType = WHL_MinMax;
  SetUserHistoLimits();
  if( !bHistoLabel ) yWHLType = WHL_None;

  ipd = 0;
  //----- LOOP TO DOSERS TO PLOT
  for( itecs = plotDosers.begin(); itecs != plotDosers.end(); itecs++, ipd++ ) {
    mdpdd* data1 = (*itecs).second;
    G4String doserName = (*itecs).first;
    if( verbose >= 2 ) std::cout << "@@@ DRAWING DOSER : " << doserName << " N= " << data1->size() << std::endl; 
    G4bool bIsFirst = false;
    if(itecs == plotDosers.begin()) {
      bIsFirst = true;
    }
   
    //--- MAKE HISTO LIMITS A BIT WIDER
    WidenHistoLimits(0.06*nPlots, xWHLType, yWHLType );
    //    WidenHistoLimits(0.05, xWHLType, WHL_Max ); // PRES
       
    //---- FOR FIRST DOSER PINT EMPTY HISTOGRAMS AND DEFINE PAD
    if( bIsFirst ) {
      //-      theCanvas->Draw();
      theCanvas->cd();
      //---- FOR 2 DOSER'S PRINT BOTH AND BELOW DIVISION
      TPad* pad1 = 0;
      if( plotDosers.size() == 2 && doseSet->PlotDivided() ) {
	//       TPad* pad1 = new TPad("","",0.,0.,1.,0.25);
	pad1 = new TPad("","",Definitions::padLeftMargin,0.25,1.-Definitions::padRightMargin,1.);
	pad1->Draw();
	pad1->cd();
	Definitions::yAxisTitleOffset = GetDrawingData("yAxisTitleOffset",1.3);
	Definitions::xAxisTitleSize = GetDrawingData("xAxisTitleSize",0.05);
	Definitions::yAxisTitleSize = GetDrawingData("yAxisTitleSize",0.05);
      } else {
	//       TPad* pad1 = new TPad("","",0.,0.,1.,0.25);
	pad1 = new TPad("","",Definitions::padLeftMargin,0.,1.-Definitions::padRightMargin,1.);
	pad1->Draw();
	pad1->cd();
	Definitions::yAxisTitleOffset = GetDrawingData("yAxisTitleOffset",1.3);
	Definitions::xAxisTitleSize = GetDrawingData("xAxisTitleSize",0.05);
	Definitions::yAxisTitleSize = GetDrawingData("yAxisTitleSize",0.05);
      }
      //--- BUILD HISTO NAMES
      if( doserName.find("PDD") != std::string::npos || doserName.find("Z_merged") != std::string::npos ) {
	Definitions::xAxisTitle = "Depth (mm)";
      }
      if( doserName.find("Prof") != std::string::npos || doserName.find("Y_merged") != std::string::npos ) {
	Definitions::xAxisTitle = "Off-axis position (mm)";
      }
    //    TH1F* hist = new TH1F("test","test",100,0.,100.);
    //  hist->Draw();
    //    PrintGif("h9057");
      //  TH1F* hist2 = new TH1F("test2","test2",100,0.,200.);
      //    hist2->Draw();
      if( DoseData::verbosity >= 2) G4cout << "@@@ bIsFirst DrawEmptyHistogram "<< G4endl;
      his = DrawEmptyHistogram( "", Definitions::xDataMin, Definitions::xDataMax, Definitions::yDataMin, Definitions::yDataMax, pad1 );
      //	  G4cout << " YDATAMIN0 " <<  Definitions::yDataMin << G4endl;
    }
    //     PrintGif("h9057");
    //     return;
    
    //--- SET COLOR FOR THESE DOSER FROM METADATA FILE
    std::map<G4String,G4int>::const_iterator itecol = theIdxColors.find((*itecs).first);
    G4int iColor = (*itecol).second;
    //    G4cout << ipd << " CHOOSE COLOR0 " << (*itecs).first << " " << iColor << G4endl; //GDEB
    if( iColor == -1 ) iColor = ipd;
    EColor color = GetColor(iColor);
    //    G4cout << ipd << " CHOOSE COLOR " << (*itecs).first << " " << iColor << G4endl; //GDEB
    //    EliminateOutOfLimits(his, data1 );
    if( DoseData::verbosity >= 2) G4cout << "@@@@@ DrawHisto: N " << data1->size() << G4endl;

    int nHistoTitles = GetDrawingData("nHistoTitles", 1000);
    DrawHisto( data1, color, 20+(ipd%nHistoTitles)*1, doserName );
    xInit = Definitions::xHistoLabel*theCanvas->GetWw()/500;
    //    xInit = 350*theCanvas->GetWw()/500; 
    yInit = (Definitions::yHistoLabel-ipd*10)*theCanvas->GetWh()/500;
    std::string dataType = GetDrawingDataMulti("histoGlobalName", ipd%nHistoTitles);
    if( bHistoLabel ) {     // PRINT DOSER LABEL OR Data/MC
      DrawWordAndMarker( his, doserName, xInit, yInit, color, 20+(ipd%nHistoTitles)*1, Definitions::markerTextSize );
    } else {
      //      color = GetColor(ipd%nHistoTitles);
      xInit = Definitions::xHistoLabel*theCanvas->GetWw()/500;
      yInit = (Definitions::yHistoLabel-ipd*20)*theCanvas->GetWh()/500;
      if( ipd < nHistoTitles ) DrawWordAndMarker( his, dataType, xInit, yInit, color, 20+(ipd%nHistoTitles)*1, Definitions::markerTextSize*2 );
    }    
    //   DrawGlobalTitle(his, hisFileName);
    DrawGlobalTitle(his, hisFileName);
    //    DrawXTitle(his,GetDrawingDataString("xAxisTitle"));
    //    DrawYTitle(his,GetDrawingDataString("yAxisTitle"));
    
  } // ENDED DOSER LOOP

  //----- MAKE STATISTICAL COMPARISON
  G4bool bStatGI = GetDrawingData("bStatGI", 0);
  G4bool bStatChi2 = GetDrawingData("bStatChi2", 0);
  if( plotDosers.size() == 2 ) {
    bStatGI = 1; 
    bStatChi2 = 1;
  }
  bStatGI = 0; 
  bStatChi2 = 0;
  G4bool bStatComp = bStatGI || bStatChi2;
  if( bStatComp ) {
    G4double theStatTextSize = GetDrawingData("statTextSize",0.03);
    vpsmdpdd::const_iterator itecs2; // loop to dosers in file
    EColor color;
    ipd = 0;
    for( itecs = plotDosers.begin(); itecs != plotDosers.end(); itecs += 2, ipd += 2) {
      itecs2 = itecs; 
      itecs2++;
      mdpdd* data1 = (*itecs).second;
      mdpdd* data2 = (*itecs2).second;
      if( DoseData::verbosity >= 1) G4cout << "@@@@@@@ START STATISTICAL COMPARISON: " <<(*itecs).first << " vs. " << (*itecs2).first << G4endl;
      mdpdd* dataDiv = new mdpdd;
      if( bStatChi2 ) {
	CompStatData cs = CompareStatsChi2( data1, data2, *dataDiv ); // CALCULATE DIVISION DATA
	xInit = 220*theCanvas->GetWw()/500;
	yInit = 15*theCanvas->GetWh()/500;
	color = GetColor(0);
	DrawWordInPave( his, "#chi^{2}= "+GmGenUtils::ftoa(cs.chi2)+"/"+GmGenUtils::ftoa(cs.ndof)+ " p=" + GmGenUtils::ftoa(cs.pValue), xInit, yInit, color, theStatTextSize );
      
      //    G4cout << " bCHI2 " << xInit << " " << yInit << G4endl;
      //    G4cout << "TOTAL CHI2: " << GmGenUtils::ftoa(cs.chi2)+"/"+GmGenUtils::ftoa(cs.ndof)<< " p-value= " << cs.pValue << G4endl; 
	if( DoseData::verbosity >= 1) G4cout << "@@@@ CHI2 p-value= " << cs.pValue << " CHI2: " << GmGenUtils::ftoa(cs.chi2)+"/"+GmGenUtils::ftoa(cs.ndof) << " " << hisFileName << G4endl;
      }    

      //-    theDistStep = 0.1/10.; 
      G4String setName = (*itecs).first;
      // VARIAN.prof15x15.data.txt_Profile15x15.d1.4.data
      //    G4cout << " SET NAME " << setName << " FIELD " << fieldStr <<  G4endl;
      if( bStatGI ) {
	if( setName.find("Prof") != std::string::npos ) {
	  // only bins inside (-lim,+lim)
	  size_t ip1 = setName.find(".");  
	  size_t ip2 = setName.find("x",ip1+1);  
	  std::string fieldStr = setName.substr(ip1+5,ip2-ip1-5);
	  if( setName.find("data") == std::string::npos ) { 
	    // prod/dose.standard.10.5.8001.0..0..0..9055_ALL.root 
	    ip1 = setName.find(".");  
	    ip1 = setName.find(".",ip1+1);  
	    ip2 = setName.find(".",ip1+1);  
	    fieldStr = setName.substr(ip1+1,ip2-ip1-1);
	  }
	  double field = atof(fieldStr);
	  if( field == 0 ) {
	    G4Exception("PlotDosers",
			"",
			FatalException,
			(G4String("Field size not found for Profile, no gamma index calculation is possible " + fieldStr + " from " + setName).c_str()));
	  }
	  if( DoseData::verbosity >= 3) G4cout << "@@@@ Field to define gamma index limits " << fieldStr << " = " << field << " from " << setName << " ip1 " << ip1 << " ip2 " << ip2 << G4endl;
	  theGMinX = -field*8.;
	  theGMaxX = -theGMinX;	
	} else {
	  // PDD CENTRAL LIMITS
	  //	  theGMinX = -350.; 
	  //	  theGMaxX = -50.;
	  theGMinX = 0.; 
	  theGMaxX = 350.;
	}
	std::vector<pdd> thePCMDLimits;
	thePCMDLimits.push_back(pdd(0.5,1.));
	//    thePCMDLimits.push_back(pdd(1.,1.));
	//   thePCMDLimits.push_back(pdd(1.,2.));
	thePCMDLimits.push_back(pdd(2.,2.));
	std::vector<pdd>::const_iterator itepm, itepml;
	yInit = 42*theCanvas->GetWh()/500;
	//    yInit = 370*theCanvas->GetWh()/500;
	color = GetColor(0);
	//--- GET GAMMA INDICES 
	for( itepml = thePCMDLimits.begin(); itepml != thePCMDLimits.end(); itepml++ ) {
	  double perCentLim = (*itepml).first;
	  double minDistLim = (*itepml).second;
	  TH1F* histoGI = getGammaIndex1CD( data1, data2, perCentLim, minDistLim );
	  // get gamma that correspond to 95%
	  G4int nGamma = theGammaFinal.size();
	  std::multiset<double>::const_iterator iteg;
	  G4double pval = 0.95;
	  G4double gammaPVal = 0.;
	  G4double jj = 0;
	  for( iteg = theGammaFinal.begin(); iteg != theGammaFinal.end(); iteg++, jj++ ) {	
	    //	std::cout << jj << " PVAL " << jj/nGamma << " = " << *iteg << std::endl; //GDEB
	    if( jj/nGamma > 0.95 ) {
	      gammaPVal = *iteg;
	      break;
	    }
	  }
	  G4double mean = histoGI->GetMean();
	  mean = int(mean*100)/100.;
	  G4double rms = histoGI->GetRMS(); 
	  rms = int(rms*100)/100.;
	  std::cout << (*itecs).first << " " << perCentLim << "% / " << minDistLim << "mm : MEAN GAMMA INDEX " << histoGI->GetMean() << " +- " << histoGI->GetRMS() << " P" << pval*100 << "= " << gammaPVal <<  std::endl;
	  //      xInit = 375*theCanvas->GetWw()/500;
	  if( plotDosers.size() == 2 ) {
	    xInit = 220*theCanvas->GetWw()/500;
	  } else {
	    G4String xInitStr = GetDrawingDataMulti("GIxInit", ipd/2);
	    if( xInitStr != "" ) {
	      xInit = GmGenUtils::GetValue(xInitStr)*theCanvas->GetWh()/500;
	    }
	    //	    xInit = GetDrawingData("GIxInit",220*theCanvas->GetWw()/500)*theCanvas->GetWw()/500;
	    if( itepml == thePCMDLimits.begin() ) {
	      G4String yInitStr = GetDrawingDataMulti("GIyInit", ipd/2);
	      if( yInitStr != "" ) {
		yInit = GmGenUtils::GetValue(yInitStr)*theCanvas->GetWh()/500;
	      }
	    }
	  }
	  //      xInit = 290*theCanvas->GetWw()/500;
	  DrawWordInPave( his, "G" + GmGenUtils::ftoa(perCentLim)+"/"+GmGenUtils::ftoa(minDistLim)+ ": " + GmGenUtils::ftoa(mean) + "+-" + GmGenUtils::ftoa(rms) + " P" + GmGenUtils::itoa(pval*100) + "= " + GmGenUtils::ftoa(int(gammaPVal*100)/100.), xInit, yInit, color, theStatTextSize );
	  yInit-= 11*theStatTextSize/0.02*theCanvas->GetWh()/500;
	}
      }
      
      if( doseSet->PlotDivided() ) {
	xInit = 30*theCanvas->GetWw()/500;
	yInit = 378*theCanvas->GetWh()/500;
	G4bool bIsFirst = true;
	//      G4int ic = 0;
	SetHistoLimits( dataDiv, bIsFirst );
	SetUserHistoLimits();
	WidenHistoLimits(0.05*nPlots, xWHLType, yWHLType );
	Definitions::yDataMin = GetDrawingData("divYMin",0.95);
	Definitions::yDataMax = GetDrawingData("divYMax",1.05);
	TPad* pad2 = new TPad("pad2","pad2",Definitions::padLeftMargin,0.05,1.-Definitions::padRightMargin,0.25);
	theCanvas->cd();
	pad2->Draw("sames");
	pad2->cd();
	Definitions::yAxisTitle = "Dose ratio";
	Definitions::xAxisTitleOffset = 1.;
	Definitions::yAxisTitleOffset = 0.25;
	Definitions::xAxisTitleSize = 0.12;
	Definitions::yAxisTitleSize = 0.15;
	Definitions::xAxisLabelSize = 0.1;
	Definitions::yAxisLabelSize = 0.1;
	his = DrawEmptyHistogram( "", Definitions::xDataMin, Definitions::xDataMax, Definitions::yDataMin, Definitions::yDataMax, pad2 );
	color = GetColor(0);
	//	EliminateOutOfLimits(his, dataDiv );
	DrawHisto( dataDiv, color, 20, "" );
	//-      if( bHistoLabel ) DrawWordAndMarker( his, doserName, xInit, yInit, color, 20+ic, 0.02 );
	DrawGlobalTitle(his, hisFileName);
	//      DrawXTitle(his,GetDrawingDataString("xAxisTitle"));
	//      DrawYTitle(his,Definitions::yAxisTitle);
	TAxis* xAxis = his->GetXaxis();
	TLine* line = new TLine( xAxis->GetXmin(), 1., xAxis->GetXmax(), 1.); // HORIZ. LINE AT 1
	line->Draw();
      }
      if( plotDosers.size() != 2 ) {
	G4String extraText = GetDrawingDataMulti("extraText", ipd/2);
	xInit = GetDrawingData("extraTextxInit",220*theCanvas->GetWw()/500)*theCanvas->GetWw()/500;
	G4String yInitStr = GetDrawingDataMulti("extraTextyInit", ipd/2);
	if( yInitStr != "" ) {
	  yInit = GmGenUtils::GetValue(yInitStr)*theCanvas->GetWh()/500;
	}
     	//      xInit = 290*theCanvas->GetWw()/500;
	G4double theExtraTextSize = GetDrawingData("extraTextSize", theStatTextSize);
	DrawWordInPave( his, extraText, xInit, yInit, color, theExtraTextSize );
      }

    } // loop to Dosers for StatComp
  }

  //----- FOR ROOT gif FILE NAME, IT CANNOT HAVE ':' NOR '$'
  size_t icc = -1;    
  for(;;) {
    icc = hisFileName.find(":",icc+1);
    if( icc == std::string::npos ) {
      icc = hisFileName.find("$",icc+1);
      if( icc == std::string::npos ) {
	break;
      }
    }
    hisFileName[icc]='_';
    std::cout << "HISTOGRAM FILE NAME : DELETED " << icc << " " << hisFileName << std::endl;
  }
  //    PrintGif(hisFileName);
  //  std::cout << " BREAK " << std::endl;
  //  return;
  //#ifndef PAPER
  //  PrintGif(hisFileName);
  //#else 

  PrintGif(hisFileName);

}

//-----------------------------------------------------------------------------
DoserData* FindDoserData( G4String fileName , G4String doserName )
{
  DoserData* doserData = 0;

  std::vector<FileData*>::const_iterator itefd; 
  for( itefd = theFileData.begin(); itefd != theFileData.end(); itefd++ ) {
    FileData* fileData = (*itefd);
    if( fileData->GetFileName() == fileName ) {
      size_t nsco = fileData->GetNumberOfDosers();
      for( size_t ii = 0; ii < nsco; ii++ ) {
	DoserData* sd = fileData->GetDoserData(ii);
	if( sd->GetName() == doserName ) {
	  doserData = sd;
	  break;
	}
      }
    }
  }
  if( doserData == 0 ) {
    G4cerr << "@@ LOOKING FOR FILE: " << fileName << " DOSER: " << doserName << G4endl;
    for( itefd = theFileData.begin(); itefd != theFileData.end(); itefd++ ) {
      FileData* fileData = (*itefd);
     G4cerr << "@@  FILE: " << fileData->GetFileName() <<  " N DOSES " << fileData->GetNumberOfDosers() << G4endl;
     size_t nsco = fileData->GetNumberOfDosers();
      for( size_t ii = 0; ii < nsco; ii++ ) {
	DoserData* sd = fileData->GetDoserData(ii);
	G4cerr << " FILE: " << fileData->GetFileName() << " DOSER: " << sd->GetName() << G4endl;
      }
    }
    G4Exception("FindDoserData",
		"",
		//		JustWarning,
		FatalException,
		(G4String("Doser not found " + doserName + " in file " + fileName).c_str()));

  } else {
  
    if( DoseData::verbosity >= 2) G4cout << " FindDoserData " << fileName << " " << doserName << " =N " << doserData->GetNDoses() << G4endl;
  }
  return doserData;
}


void SetUserHistoLimits()
{
  if( theHYMin != -DBL_MAX ) {
    Definitions::yDataMin = theHYMin;
    if( theHYMax != -DBL_MAX ) {
      yWHLType = WHL_None; 
    } else {
      yWHLType = WHL_Max; 
    }    
  }
  if( theHYMax != -DBL_MAX ) {
    Definitions::yDataMax = theHYMax;
    if( theHYMin == -DBL_MAX ) {
      yWHLType = WHL_Min; 
    }    
  }
  if( theHXMin != -DBL_MAX ) {
    Definitions::xDataMin = theHXMin;
    if( theHXMax != -DBL_MAX ) {
      xWHLType = WHL_None; 
    } else {
      xWHLType = WHL_Max; 
    }    
  }
  if( theHXMax != -DBL_MAX ) {
    Definitions::xDataMax = theHXMax;
    if( theHXMin == -DBL_MAX ) {
      xWHLType = WHL_Min; 
    }    
  }

  if( DoseData::verbosity >= 1) G4cout << "@@@@@ SetUserHistoLimits: " << theHXMin << " < X < " << theHXMax << " " << theHYMin << " < Y < " << theHYMax << G4endl;

}

//-----------------------------------------------------------------------------
void ReadDrawingData( G4String fileName )
{ 
  //  theDrawingData["bStatChi2"]  = "0";
  //  theDrawingData["xStatGI"]  = "200";
  //  theDrawingData["xInitHGN"] = "215";
  //  theDrawingData["yInitHGN"] = "400";

  std::vector<G4String> wl;
  G4cout << " ReadDrawingData: " << fileName << G4endl;
  GmFileIn fin = GmFileIn::GetInstance(fileName,1);
  for( G4int ii = 0;; ii++) {
    if( ! fin.GetWordsInLine(wl) ) break;
    if( wl.size() != 3 ) {
      std::cerr << " READING LINE N WORDS " << wl.size() << G4endl;
      for( size_t jj = 0; jj < wl.size(); jj++ ) {
	std::cerr << wl[jj] << " ";
      }
      std::cerr << std::endl;
      G4Exception("ReadDrawingData",
		  "",
		  FatalException,
		  "Line does not have 3 words");
    }     
    if( wl[0] == ":P" ) {
      theDrawingData[wl[1]] = wl[2];
    } else if( wl[0] == ":PM" ) {
      msvs::const_iterator items = theDrawingDataMulti.find(wl[1]);
      std::vector<G4String> data;
      if( items == theDrawingDataMulti.end() ) {
	data.push_back( wl[2] );
      } else {
	data = (*items).second;
	data.push_back( wl[2] );
      }
      theDrawingDataMulti[wl[1]] = data;
    }
  }

  if( DoseData::verbosity >= 1 ) { 
    std::map<G4String,G4String>::const_iterator ite;
    std::cout << " ReadDrawingData READ " << theDrawingData.size() << " MULTI " << theDrawingDataMulti.size() << std::endl;
    for( ite = theDrawingData.begin(); ite != theDrawingData.end(); ite++ ) {
      std::cout << " theDrawingData  " << (*ite).first << " = " << (*ite).second << G4endl; 
    }
    msvs::const_iterator items;
    for( items = theDrawingDataMulti.begin(); items != theDrawingDataMulti.end(); items++ ) {
      for( size_t jj = 0; jj < (*items).second.size(); jj++ ) {
	std::cout << " theDrawingDataMulti  " << (*items).first << " : " << jj << " = " << ((*items).second)[jj]  << G4endl; 
      }
    }
  }

}

//-----------------------------------------------------------------------------
G4double GetDrawingData( G4String dataName, G4double def ) 
{
  std::map<G4String,G4String>::const_iterator ite = theDrawingData.find(dataName);
  if( ite == theDrawingData.end() ) {
    return def; 
  } else {
    return GmGenUtils::GetValue((*ite).second);
  }
}

//-----------------------------------------------------------------------------
G4String GetDrawingDataString( G4String dataName, G4String def ) 
{
  std::map<G4String,G4String>::const_iterator ite = theDrawingData.find(dataName);
  if( ite == theDrawingData.end() ) {
    return def; 
  } else {
    return (*ite).second;
  }
}


//-----------------------------------------------------------------------------
G4String GetDrawingDataMulti( G4String dataName, G4int ipd ) 
{
  G4String dataStr = "";
  if( G4int(theDrawingDataMulti[dataName].size()) > ipd ) {
    dataStr = (theDrawingDataMulti[dataName])[ipd];
  } 
  return dataStr;
}

//-------------------------------------------------------------------
void PrintHelp()
{
  G4cout << " -f          doses compare file name " << G4endl
	 << " -fDrawingData  File containing position/size of elements in drawing "
	 << " -hXMin      Minimum X value of the histograms " << G4endl
	 << " -hXMax      Maximum X value of the histograms " << G4endl
	 << " -hYMin      Minimum Y value of the histograms " << G4endl
	 << " -hYMax      Maximum Y value of the histograms " << G4endl
	 << " -hXLog      Histogram X axis in logarithmic scale (1) or linear (0)" << G4endl
	 << " -hYLog      Histogram Y axis in logarithmic scale (1) or linear (0)" << G4endl
	 << " -bHistoLabel  Draw name of histogram from metadata file (1) or read name from fDrawingData file (0). Default = 1" << G4endl
	 << " -verb       verbosity: sets the ShieldingVerbosity" << G4endl
	 << " -indexValType  Type of treatment of dose index (get upper limit, lower limit or average " << G4endl
	 << " -help       prints the set of arguments " << G4endl;
}

