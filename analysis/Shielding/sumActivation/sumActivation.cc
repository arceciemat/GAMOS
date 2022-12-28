#include "Shielding/include/SHRadDecayChainSet.hh"
#include "Shielding/include/SHRadDecayChain.hh"
#include "Shielding/include/ShieldingVerbosity.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosRunManager/include/GmRunManager.hh"
#include "ROOTUtilities/PlotData/DrawHisto.C"
#include "ROOTUtilities/Activation/readExemption.C"

#include <fstream>

std::map<G4String,SHRadDecayChainSet*> theIsotopeSets;
void ReadActivityResults(G4String fileName);
void PrintData(std::ofstream& out);
void PlotData();
void PrintHelp();
void ReadPeriods( const G4String& fileName );
void ReadTimes(G4String fileName);
std::map<G4double,G4bool> thePeriods;
std::vector<G4double> theTimes;
G4int theNPerPage;
G4double theHYMin, theHYMax;
G4double theTimeUnitValue;
G4String theTimeUnit;
G4bool bExemption;
G4String theExemptionFile;
G4double theMinActivity;

//-----------------------------------------------------------------------------
int main(int argc,char** argv) 
{
  theNPerPage = 5;
  theHYMin = -1;
  theHYMax = -1;
  theTimeUnit = "second";
  bExemption = false;
  theExemptionFile = "exemption.EU.txt";
  theMinActivity = -DBL_MAX;
  Definitions::bLogX = true;
  Definitions::bLogY = true;
  
  G4String theFileName = "";
  G4String theFileListName = "";
  G4String theFileNameOut = "activation.out";
  G4String theFileNamePeriods = "";
  G4String theFileNameTimes = "";

  if( argc == 2 ) {
    if( G4String(argv[1]) == "-h" || G4String(argv[1]) == "-help" ) {
       PrintHelp();
     } else {
       theFileName = argv[1];
     }
  } else {
    if(argc%2 != 1) { 
      G4Exception("sumActivation",
		  "Error",
		  FatalErrorInArgument,
		  "WRONG NUMBER OF ARGUMENTS: THEY MUST BE -XX1 VAL_XX1 -XX2 VAL_XX2 ... ");
    }
    for( G4int ii = 1; ii < argc; ii+=2 ){
      G4String argvstr = argv[ii];
      if( argvstr == "-f" ) {
	theFileName = argv[ii+1];
      } else if( argvstr == "-fl" ) {
	theFileListName = argv[ii+1];
      } else if( argvstr == "-fOut" ) {
	theFileNameOut = argv[ii+1];
      } else if( argvstr == "-fPeriods" ) {
	theFileNamePeriods = argv[ii+1];
      } else if( argvstr == "-fTimes" ) {
	theFileNameTimes = argv[ii+1];
      } else if( argvstr == "-verb" ) {
	G4String verbstr = G4String(argv[ii+1]);
	if(verbstr == "test" ) {
	  verbose = 3;
	} else if(verbstr == "debug" ) {
	  verbose = 2;
	} else if(verbstr == "info" ) {
	  verbose = 1;
	} else if(verbstr == "warning" ) {
	  verbose = 0;
	} else if(verbstr == "error" ) {
	  verbose = -1;
	} else {
	  G4cerr << " VERBOSITY " << argv[ii+1]  << G4endl;
	  G4Exception("sumScores",
		  "Error",
		  FatalErrorInArgument,
		      "verbosity wrong");
	}
	verbstr = "ShieldingVerbosity " + verbstr;
	GmRunManager* runManager = new GmRunManager();
	runManager->SelectVerbosity(verbstr);
      } else if( argvstr == "-nIsotopesPerPage" ) {
	theNPerPage = GmGenUtils::GetInt(argv[ii+1]);
      } else if( argvstr == "-timeUnit " ) {
	theTimeUnit = argv[ii+1];
      } else if( argvstr == "-hYMin" ) {
	theHYMin = GmGenUtils::GetValue(argv[ii+1]);
      } else if( argvstr == "-hYMax" ) {
	theHYMax = GmGenUtils::GetValue(argv[ii+1]);
      } else if( argvstr == "-hLogX" ) {
	Definitions::bLogX = GmGenUtils::GetBool(argv[ii+1]);
      } else if( argvstr == "-hLogY" ) {
	Definitions::bLogY = GmGenUtils::GetBool(argv[ii+1]);
      } else if( argvstr == "-exemption" ) {
	theExemptionFile = argv[ii+1];
	bExemption = true;
      } else if( argvstr == "-minActivity" ) {
	theMinActivity = GmGenUtils::GetValue(argv[ii+1]);
      } else {
	PrintHelp();
	G4Exception("sumActivation",
		    "Wrong argument",
		    FatalErrorInArgument,
		    (G4String("ARGUMENT: ") + argvstr).c_str());
      }
    }
  }
  theTimeUnitValue = GmGenUtils::GetValue(theTimeUnit);

  if( theFileName != "" && theFileListName != "" ) {
    G4Exception("sumActivation",
		"Error",
		FatalException,
		"You have asked at the same time for a file and for a list of files, do not use -f and -fl at the same time");
  }
  if( theFileName == "" && theFileListName == "" ) {
    G4Exception("sumActivation",
		"Error",
		FatalException,
		"You have  not asked for a file nor for a list of files, please use -f or -fl"); }

  if( theFileNamePeriods != "" ) {
    ReadPeriods( theFileNamePeriods );
  }

  if( theFileNameTimes != "" ) {
    ReadTimes( theFileNameTimes );
  }

  std::vector<G4String> wl;
  
  // Read file
  if( theFileName != "" ) {
    GmFileIn fin = GmFileIn::GetInstance(theFileName);
    G4cout << "@@@@@@@ Reading FILE: " << theFileName << G4endl;
    ReadActivityResults( theFileName );
  } else { 
     GmFileIn finlis = GmFileIn::GetInstance(theFileListName);
     // Read each file in file list
     int ii;
     for( ii = 0;; ii++) {
       if( ! finlis.GetWordsInLine(wl) ) break;      
       if( wl.size() != 1 ) {
	 G4Exception("sumActivation",
		     "Wrong atgument",
		     FatalErrorInArgument,
		     G4String("YOUR FILE_LIST HAVE TOO MANY WORDS IN LINE NUMBER: " + GmGenUtils::itoa(ii+1)).c_str());
       }
       
       G4cout << "@@@@@@@ Reading FILE: " << wl[0]  << G4endl;
       ReadActivityResults( wl[0] );
     }
  }

  std::ofstream fout(theFileNameOut.c_str());
  PrintData(fout);

  PlotData();

  return 0;
}

//-----------------------------------------------------------------------------
void ReadActivityResults(G4String fileName)
{
  std::vector <G4String> wl;

  GmFileIn fin = GmFileIn::GetInstance(fileName); 
  fin.SetCheckQuotes( false );
  //%%%%%% SHActivationUA_inCameraF:  NUMBER OF EVENTS PER SECOND = 2.28311e+10 = 1e+07 events in 438000 nanoseconds 
  //%%% SHActivationUA_inCameraF: END_PRODUCTION: 3.1536e+08 sec  TIME: 2.592e+06 sec : Nb93[30.8] : ACTIVITY= 490.959 decays/sec   LIFETIME=7.34332e+08 sec   NPART_IN_JOB= 88   NIsotopes= 5.20131e+11 
  SHRadDecayChainSet* isotSet = 0;
  std::map<G4String,SHRadDecayChainSet*>::const_iterator ites;
  for(;;){
    if( !fin.GetWordsInLine(wl) ) break;
    //    std::cout << " LINE " << wl.size() << " " << wl[0] << std::endl;
    if( wl.size() > 1 && wl[0].find("%%") != G4String::npos && wl[1].find("SHActivation") != G4String::npos ) {
      ites =  theIsotopeSets.find(wl[1]);
      if( ites !=  theIsotopeSets.end() ) {
	isotSet = (*ites).second;
	//       	G4cout << " OLD isotSet " << isotSet << " " << wl[1] << G4endl;
      } else {
	isotSet = new SHRadDecayChainSet( wl[1] );
	theIsotopeSets[wl[1]] = isotSet;
	// one new set if there are SHActivationUA with different filters
	G4cout << " NEW isotSet " << isotSet << " " << wl[1] << G4endl;
      } 
      if( wl[2] == "TIME=" && GmGenUtils::GetValue(wl[7]) < theMinActivity ) {
	//	G4cout << " SKIP ACTIVITY " << wl[7] << " for " << wl[4] << " " << wl[5] << " " << wl[6] << G4endl; //GDEB
	continue;
      }
      //      G4cout << "SETS " << theIsotopeSets.size() << G4endl; //GDEB
      isotSet->AddData(wl);

    }
  }
  
}
 

//------------------------------------------------------------------
void ReadPeriods( const G4String& fileName )
{

  GmFileIn fin = GmFileIn::GetInstance(fileName);
  std::vector<G4String> wl;
  G4double periodSum = 0.;
  for(;;){
    if( ! fin.GetWordsInLine(wl)  ) break;
    if( wl.size() != 2 ) {
      G4Exception("sumActivation::ReadPeriods",
		  "Error",
		  FatalException,
		  (G4String("Problem reading file, there should be two words per line, there are ") + GmGenUtils::itoa(wl.size())).c_str());
    }
    periodSum += GmGenUtils::GetValue(wl[0]);
    thePeriods[ periodSum ] = GmGenUtils::GetBool(wl[1]); 
  }
}

//-----------------------------------------------------------------------------
void ReadTimes(G4String fileName)
{
  std::vector <G4String> wl;

  GmFileIn fin = GmFileIn::GetInstance(fileName); 
  for(;;){
    if( !fin.GetWordsInLine(wl) ) break;
    for( unsigned int ii = 0; ii < wl.size(); ii++) {
      theTimes.push_back( GmGenUtils::GetValue(wl[ii]) );
    }
  }
}

//-----------------------------------------------------------------------------
void PrintData(std::ofstream& out)
{
  std::map<G4String,SHRadDecayChainSet*>::const_iterator ite;
  G4String printedIsotNames = "";

  for( ite = theIsotopeSets.begin(); ite != theIsotopeSets.end(); ite++) {
    SHRadDecayChainSet* isotSet = (*ite).second;
    if( thePeriods.size() != 0 ) isotSet->SetPeriods(thePeriods);
    if( theTimes.size() != 0 ) isotSet->SetTimes(theTimes);
    //-  isotSet->CheckLifeTimes();
    isotSet->PrintData(out);
  }
}

//-----------------------------------------------------------------------------
void PlotData()
{
  if( bExemption ) {
    theExemptionFile = GmGenUtils::FileInPath( theExemptionFile);
    readExemption( theExemptionFile );
  }

  std::map<G4String,SHRadDecayChainSet*>::const_iterator ite;
  G4String printedIsotNames = "";

  if( verbose >= 2 ) G4cout << " PlotData N ISOTOPE SETS " << theIsotopeSets.size() << G4endl;
  for( ite = theIsotopeSets.begin(); ite != theIsotopeSets.end(); ite++) {
    SHRadDecayChainSet* isotSet = (*ite).second;
    G4String setName = (*ite).first;
    //--- Change periods and times of files for those in input
    if( thePeriods.size() != 0 ) isotSet->SetPeriods(thePeriods);
    if( theTimes.size() != 0 ) isotSet->SetTimes(theTimes);
    //    isotSet->CheckLifeTimes();
    //build plotData: isotope : time - activity 
    msmdpdd plotData = isotSet->GetPlotData(theTimeUnitValue);
    //---- Order data by maximum of activity
    msmdpdd::const_iterator itecs; // loop to chains in each set
    mdpdd::iterator itec; // loop to time-activities  in each chain
    std::multimap<G4double, msmdpdd::const_iterator> orderedData;
    if( verbose >= 2 ) G4cout << " PlotData N ISOTOPES " << plotData.size() << G4endl;
    for( itecs = plotData.begin(); itecs != plotData.end(); itecs++ ) {
      mdpdd* data1 = (*itecs).second;
      /*      for( itec = data1->begin(); itec != data1->end(); itec++ ) {
	(*itec).second.first *= 1.e-5;
	}*/
      if( theMinActivity != -DBL_MAX ) {
	mdpdd* data1new = new mdpdd;
	for( mdpdd::iterator itemd = data1->begin(); itemd != data1->end(); itemd++) {
	  if( (*itemd).second.first >= theMinActivity ) {
	    (*data1new)[(*itemd).first] = (*itemd).second;
	  }
	  //	  delete data1new;
	}
	*data1 = *data1new;
      }
      SetHistoLimits( data1, (itecs == plotData.begin() ) );
      G4double maxActiv = 0.;
      for( itec = data1->begin(); itec != data1->end(); itec++ ) {
	maxActiv = std::max(maxActiv,(*itec).second.first);
      }

      orderedData.insert(std::multimap<G4double, msmdpdd::const_iterator>::value_type(maxActiv, itecs));
      if( verbose >= 2 ) G4cout << "sumActivation: ORDERING DATA " << (*itecs).first << " = " << maxActiv << G4endl;
    }
    if( theHYMin != -1 ) {
      Definitions::yDataMin = theHYMin;
      if( theHYMax == -1 ) WidenHistoLimits(0.05, WHL_MinMax, WHL_Max);
    } else if( theHYMax != -1 ) {
      Definitions::yDataMax = theHYMax;
      if( theHYMin == -1 ) WidenHistoLimits(0.05, WHL_MinMax, WHL_Min);
    } else {
      WidenHistoLimits(0.05, WHL_MinMax, WHL_MinMax);
    }
    //build histo names


    G4int ii = 0;
    G4String hisFileName = setName + ".1.";
    std::multimap<G4double, msmdpdd::const_iterator>::reverse_iterator iteod;
    TH1F* his = 0;
    double xInit = 30;
    double yInit = 375;
    double xInitExemp = 30;
    for( iteod = orderedData.rbegin(); iteod != orderedData.rend(); iteod++, ii++ ) {
      itecs = (*iteod).second;
      //    for( itecs = plotData.begin(); itecs != plotData.end(); itecs++, ii++ ) {
      if( verbose >= 2 ) G4cout << "sumActivation: LOOPING ORDERED " << (*iteod).first << " " << (*itecs).first << G4endl;
      if( ii % theNPerPage == 0 ) {
	if( bExemption ) xInitExemp = 30;
	xInit = 30;
	yInit = 375;
	his = DrawEmptyHistogram( "", Definitions::xDataMin, Definitions::xDataMax, Definitions::yDataMin, Definitions::yDataMax, theCanvas );
	hisFileName = setName + "." + GmGenUtils::itoa(ii/theNPerPage+1)+".";
      }
      mdpdd* data1 = (*itecs).second;
      EColor color = GetColor(ii%theNPerPage);
      G4String chainName = (*itecs).first;
      // eliminate [0.0]
      size_t iae = chainName.find("[0.0]");
      while (iae != std::string::npos) {
	chainName = chainName.substr(0,iae);
	iae = chainName.find("[0.0]");
      } 
      EliminateOutOfLimits(his, data1 );
      DrawHisto( data1, color, 20+ii%theNPerPage, (*itecs).first );
      if( bExemption ) DrawExemptionLine( his, chainName, color, xInitExemp );
      DrawWordAndMarker( his, chainName, xInit, yInit, color, 20+ii%theNPerPage );
      //      std::cout << " PRINTING chaing " << chainName << " " << xInit << " " << yInit << G4endl; //GDEB
      if( ii % theNPerPage != theNPerPage -1 && ii != G4int(orderedData.size())-1) {
	hisFileName += chainName + "_";
      } else {
	hisFileName += chainName;
	//-  his->SetName(hisFileName.c_str());
	//-  his->SetTitle(hisFileName.c_str());
	DrawGlobalTitle(his, hisFileName);
	DrawXTitle(his,"Time ("+theTimeUnit+")");
	DrawYTitle(his,"Activity (decays/second)");
	PrintFig(hisFileName);
      }
    }
    /*    if( ii % theNPerPage != 0 ) {
      theCanvas->cd();
      PrintGlobalTitle(his, hisFileName);
      PrintXTitle(his,"Time ("+theTimeUnit+")");
      PrintYTitle(his,"Activity (decays/second)");
      //PrintGlobalTitle(his, "TRYTRYTRY");
      PrintFig(hisFileName);
      } */
    //    isotSet->PrintData(out);
  }

}

//-----------------------------------------------------------------------------
void PrintHelp()
{
  G4cout << " -f          activation file name " << G4endl
	 << " -f          activation file list name " << G4endl
	 << " -fOut       output file name " << G4endl
	 << " -fPeriods   name of periods file (when isotope generation is on or off)" << G4endl
	 << " -fTimes     name of times file (times when activity will be printed/plotted)" << G4endl
	 << " -nIsotopesPerPage     Number of isotopes to merge on a plot " << G4endl
	 << " -timeUnit   The unit for plotting the times " << G4endl
	 << " -hYMin      Minimum Y value of the histograms " << G4endl
	 << " -hYMax      Maximum Y value of the histograms " << G4endl
	 << " -hLogX      Histogram X axis in logarithmic scale (1) or linear (0)" << G4endl
	 << " -hLogY      Histogram Y axis in logarithmic scale (1) or linear (0)" << G4endl
	 << " -exemption Draw isotope exemption value as a line " << G4endl
	 << " -verb       verbosity: sets the ShieldingVerbosity" << G4endl
	 << " -help       prints the set of arguments " << G4endl;
}
