#include "FileData.hh"
#include "ScorerData.hh"
#include "ScoreData.hh"
#include "GAMOSFileData.hh"
#include "MCNPFileData.hh"

#include <fstream>
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosRunManager/include/GmRunManager.hh"
#include "ROOTUtilities/PlotData/DrawHisto.C"
#include "ROOTUtilities/PlotData/PlotUtils.C"

void PrintHelp();
void PlotData( FileData* fd );
std::vector<FileData*> theFileData;
G4double theHXMin, theHXMax;
G4double theHYMin, theHYMax;

#include "ReadScores.icc"

typedef std::map<double,pdd> mdpdd;

//-------------------------------------------------------------------
int main(int argc,char** argv) 
{
  theHXMin = -DBL_MAX;
  theHXMax = -DBL_MAX;
  theHYMin = -DBL_MAX;
  theHYMax = -DBL_MAX;
  Definitions::bLogX = true;
  Definitions::bLogY = true;

  G4String theFileName = "";
  G4String theFileListName = "";
  G4String theFileType = "GAMOS";
  G4String theFileNameOut = "sumScores.out";

  G4bool bHistosData = true;

  if( argc == 2 ) {
    if( G4String(argv[1]) == "-h" || G4String(argv[1]) == "-help" ) {
      PrintHelp();
    } else {
      theFileListName = argv[1];
    }
  } else if(argc == 3 && argv[1][0] != '-' ) {
    theFileListName = argv[1];
    theFileNameOut = argv[2];
  } else {
    if(argc%2 != 1) { 
      G4Exception("sumScores",
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
      } else if( argvstr == "-fType" ) {
	theFileType = argv[ii+1];
      } else if( argvstr == "-fOut" ) {
	theFileNameOut = argv[ii+1];
      } else if( argvstr == "-verb" ) {
	G4String verbstr = G4String(argv[ii+1]);
 	if(verbstr == "test" ) {
	  ScoreData::verbosity = 3;
	  verbose = 3;
	} else if(verbstr == "debug" ) {
	  ScoreData::verbosity = 2;
	  verbose = 2;
	} else if(verbstr == "info" ) {
	  ScoreData::verbosity = 1;
	  verbose = 1;
	} else if(verbstr == "warning" ) {
	  ScoreData::verbosity = 0;
	  verbose = 0;
	} else if(verbstr == "error" ) {
	  ScoreData::verbosity = -1;
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
      } else if( argvstr == "-hXMin" ) {
	theHXMin = GmGenUtils::GetValue(argv[ii+1]);
      } else if( argvstr == "-hXMax" ) {
	theHXMax = GmGenUtils::GetValue(argv[ii+1]);
      } else if( argvstr == "-hYMin" ) {
	theHYMin = GmGenUtils::GetValue(argv[ii+1]);
      } else if( argvstr == "-hYMax" ) {
	theHYMax = GmGenUtils::GetValue(argv[ii+1]);
      } else if( argvstr == "-hLogX" ) {
	Definitions::bLogX = GmGenUtils::GetBool(argv[ii+1]);
      } else if( argvstr == "-hLogY" ) {
	Definitions::bLogY = GmGenUtils::GetBool(argv[ii+1]);
      } else if( argvstr == "-indexValType" ) {
	G4String idxstr = G4String(argv[ii+1]);
 	if(idxstr == "upper" || idxstr == "UPPER" ) {
	  ScoreData::theIndexValType= SIVT_UPPER;
	} else if(idxstr == "middle" || idxstr == "MIDDLE" ) {
	  ScoreData::theIndexValType= SIVT_MIDDLE;
	} else if(idxstr == "lower" || idxstr == "LOWER" ) {
	  ScoreData::theIndexValType= SIVT_LOWER;
	} else {
	  G4cerr << " INDEX VAL TYPE " << idxstr << G4endl;
	  G4Exception("sumScores",
		      "Error",
		      FatalErrorInArgument,
		      (G4String("Wrong index val type, it can be upper/UPPER, middle/MIDDLE or lower/LOWER, while it is") + idxstr).c_str());
	}
      } else if( argvstr == "-bHistos" ) {
	bHistosData = G4bool(GmGenUtils::GetValue(argv[ii+1]));
      } else {
	PrintHelp();
	G4Exception("sumScores",
		    "Wrong argument",
		    FatalErrorInArgument,
		    (G4String("ARGUMENT: ") + argvstr).c_str());
      }
    }
  }

  if( theFileName != "" && theFileListName != "" ) {
    G4Exception("sumScores",
		"Error",
		FatalException,
		"You have asked at the same time for a file and for a list of files, do not use -f and -fl at the same time");
  }
  if( theFileName == "" && theFileListName == "" ) {
    G4Exception("sumScores",
		"Error",
		FatalException,
		"You have  not asked for a file nor for a list of files, please use -f or -fl"); 
  }

  std::ofstream fout(theFileNameOut);
  
  if( theFileName != "" ) {
    if( ScoreData::verbosity >= 0 ) G4cout << "@@@@@@@ Reading FILE: " << theFileName << G4endl;
    ReadScores( theFileName, theFileType );
  } else {    
    std::vector<G4String> wl;
    GmFileIn finlis = GmFileIn::GetInstance(theFileListName,true);
    
    for( G4int ii = 0;; ii++) {
      if( ! finlis.GetWordsInLine(wl) ) break;      
      
      if( wl.size() != 1 ) {	
	G4Exception("sumScores",
		    "Wrong argument",
		    FatalErrorInArgument,
		    G4String("YOUR FILE_LIST HAVE TOO MANY WORDS IN LINE NUMBER: " + GmGenUtils::itoa(ii+1)).c_str());
      }
      
      G4cout << "@@@@@@@ Reading FILE: " << wl[0] << "  of type " << theFileType << G4endl;
      ReadScores( wl[0], theFileType );
    }
  }
  
  //--- Average files 
  if( ScoreData::verbosity >= 0 ) G4cout << "STARTING TO MERGE DATA FROM N FILES: " << theFileData.size() << G4endl;
  FileData* fdAVE = 0;
  
  //  G4cout << " FILEDATA SIZE " << theFileData.size() << " TYPE " << theFileType <<  G4endl;
  std::vector<FileData*>::const_iterator ite;
  for( ite = theFileData.begin(); ite != theFileData.end(); ite++ ) {
    if( theFileType == "GAMOS" ){
      GAMOSFileData* fd = static_cast<GAMOSFileData*>(*ite);
      if( ScoreData::verbosity >= -2 ) G4cout << " ADDING FILE " << fd->GetFileName() << G4endl;
      if( ite == theFileData.begin() ) {
	fdAVE = new GAMOSFileData(*fd);
	if( ScoreData::verbosity >= -2 ) G4cout << " NSCORERS " << fd->GetNumberOfScorers() << " -> " << fdAVE->GetNumberOfScorers() << G4endl;
      } else {
	fdAVE->Add(fd);	
      }
    } else if( theFileType == "MCNP" ){
      MCNPFileData* fd = static_cast<MCNPFileData*>(*ite);
      if( ite == theFileData.begin() ) {
	fdAVE = new MCNPFileData(*fd);
	if( ScoreData::verbosity >= 2 ) G4cout << " NSCORE " << fd->GetNumberOfScorers() << " -> " << fdAVE->GetNumberOfScorers() << G4endl;
      } else {
	fdAVE->Add(fd);
      }
    } else if( theFileType == "text" || 
	       theFileType == "TEXT" ){
      TextFileData* fd = static_cast<TextFileData*>(*ite);
      if( ite == theFileData.begin() ) {
	fdAVE = new TextFileData(*fd);
	if( ScoreData::verbosity >= 2 ) G4cout << " NSCORE " << fd->GetNumberOfScorers() << " -> " << fdAVE->GetNumberOfScorers() << G4endl;
      } else {
	fdAVE->Add(fd);
      }
    }
  }
  
  fdAVE->Print(fout);

  if( bHistosData ) PlotData(fdAVE);
 
  /*t 
// PRINT ONE MCNP SCORER  
  std::vector<ScorerData*> scorerDataList = fdAVE->GetAllScorerData();
  for( size_t ii = 0; ii < scorerDataList.size(); ii++ ) {
    ScorerData* scorerD = scorerDataList[ii];
    G4cout << " SCORER NAME " << scorerD->GetName() << G4endl;
    if( scorerD->GetName() == "tally 1" ) {
      std::ofstream fout("tally1.txt");
      scorerD->Print(fout);
      fout << " end file " << G4endl;
      fout.close();
    }
  }
*/ 
  return 0;
}


//-----------------------------------------------------------------------------
void PrintHelp()
{
  G4cout << " -f          scores file name " << G4endl
	 << " -fl         scores file list name " << G4endl
	 << " -fOut       output file name " << G4endl
	 << " -hYMin      Minimum Y value of the histograms " << G4endl
	 << " -hYMax      Maximum Y value of the histograms " << G4endl
	 << " -hLogX      Histogram X axis in logarithmic scale (1) or linear (0)" << G4endl
	 << " -hLogY      Histogram Y axis in logarithmic scale (1) or linear (0)" << G4endl
	 << " -verb       verbosity: sets the ShieldingVerbosity" << G4endl
	 << " -indexValType  Type of treatment of score index (get upper limit, lower limit or average " << G4endl
	 << " -bHistos    Draw histograms or not (1)" << G4endl
	 << " -help       prints the set of arguments " << G4endl;
}

//-----------------------------------------------------------------------------
void PlotData( FileData* fd ) 
{
  G4bool bLogXOrig = Definitions::bLogX;
  G4bool bLogYOrig = Definitions::bLogY;
  msmspdd plotData = fd->GetPlotData(Definitions::bLogX);

  msmspdd::const_iterator itecs; // loop to scorers in file. One figure per page
  mspdd::const_iterator itec; // loop to scores in scorer
  for( itecs = plotData.begin(); itecs != plotData.end(); itecs++ ) {
    Definitions::bLogX = bLogXOrig;
    Definitions::bLogY = bLogYOrig;
    G4String scoreName = (*itecs).first;
    mspdd* data1 = (*itecs).second;
    mdpdd* data1d = new mdpdd;
    for( mspdd::const_iterator ite = data1->begin(); ite != data1->end(); ite++ ) {
      (*data1d)[GmGenUtils::GetValue(ite->first)] = ite->second;
    }
    if( data1d->size() == 0 ) {
      G4Exception("PlotData",
		  "No data",
		  JustWarning,
		  (G4String("No data in scorer: ") + scoreName).c_str());
      continue;
    }

    SetHistoLimits( data1d, 1 );

    WHLType xWHLType = WHL_MinMax;
    WHLType yWHLType = WHL_MinMax;
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
    WidenHistoLimits(0.05, xWHLType, yWHLType );

    //build histo names
    G4String hisFileName = scoreName;
    TH1F* his = 0;
    double xInit = 30;
    double yInit = 375;
    his = DrawEmptyHistogram( "", Definitions::xDataMin, Definitions::xDataMax, Definitions::yDataMin, Definitions::yDataMax, theCanvas );
    hisFileName = scoreName;
    EColor color = kBlack;
    EliminateOutOfLimits(his, data1d );
    DrawHisto( data1d, color, 20, (*itecs).first );
    DrawWordAndMarker( his, scoreName, xInit, yInit, color, 20 );
    DrawGlobalTitle(his, hisFileName);
    DrawXTitle(his,"Energy");
    DrawYTitle(his,"Score");
    PrintFig(hisFileName);

    G4bool bHistoFile = false;
    if( bHistoFile ) {
      G4bool bMakeFile = true;
      // Get number of bins
      double binw = 0.;
      double binw_prev = 0.;
      mdpdd::const_iterator ited1 = data1d->begin();
      mdpdd::const_iterator ited2 = data1d->begin(); ited2++;
      for( ; ited2 != data1d->end(); ited1++, ited2++ ) {
	binw = (*ited2).first - (*ited1).first;
	if( binw < 0. ) {
	  G4cerr << " !!! Making histogram of scorer " << scoreName << " : negative increase of index " << binw << " " << (*ited1).first << " -> " << (*ited2).first << G4endl;
	  G4cerr << " !!! No histogram file will be written " << G4endl;
	  bMakeFile = false;
	  break;
	}	
	if( ited1 != data1d->begin() ) {
	  if( fabs(binw-binw_prev)/binw < 1.e-6 ) {
	    G4cerr << " !!! Making histogram of scorer " << scoreName << " : different increase of index " << binw << " <> " << binw_prev << " ; " << fabs(binw-binw_prev)/binw << G4endl;
	    G4cerr << " indexes " << (*ited2).first << " - " << (*ited2).first;
	    ited1--; ited2--;
	    G4cerr << " <> indexes " << (*ited2).first << " - " << (*ited2).first << G4endl;
	    G4cerr << " !!! No histogram file will be written " << G4endl;
	    bMakeFile = false;
	  }
	}
	if( !bMakeFile ) return;
	G4int nBins = G4int((Definitions::xDataMax - Definitions::xDataMin )/binw+1);
	new TH1F( scoreName.c_str(), scoreName.c_str(), nBins, Definitions::xDataMin, Definitions::xDataMax );
	
      }
    }

  }

}
