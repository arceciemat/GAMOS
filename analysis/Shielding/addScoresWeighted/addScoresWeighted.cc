#include "ScoreSet.hh"
#include "../analysis/Shielding/sumScores/include/FileData.hh"
#include "../analysis/Shielding/sumScores/include/ScorerData.hh"
#include "../analysis/Shielding/sumScores/include/GAMOSScorerData.hh"
#include "../analysis/Shielding/sumScores/include/ScoreData.hh"
#include <fstream>
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosRunManager/include/GmRunManager.hh"
#include "ROOTUtilities/Verbosity.C"

void PrintHelp();
void ReadCompareFile( G4String fileName );
void PrintData( ScoreSet* scoreSet );
ScorerData* FindScorerData( G4String fileName , G4String scorerName );
std::vector<FileData*> theFileData;
G4double theHXMin, theHXMax;
G4double theHYMin, theHYMax;
std::vector<ScoreSet*> theScoreSets; 
std::set<pss> theFiles; // fileName - fileType

#include "../analysis/Shielding/sumScores/include/ReadScores.icc"

//-------------------------------------------------------------------
int main(int argc,char** argv) 
{

  G4String theCompareFileName = "scoreAdd.lis";

   if( argc == 2 ) {
     if( G4String(argv[1]) == "-h" || G4String(argv[1]) == "-help" ) {
       PrintHelp();
       return 0;
     } else {
       theCompareFileName = argv[1];
     }
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
	theCompareFileName = argv[ii+1];
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
	if( G4String(argv[ii+1]) == "info" ) {
	  verbose = 2;       
	}else if( G4String(argv[ii+1]) == "debug" ) {
	  verbose = 3;
	}
      } else if( argvstr == "-indexValType" ) {
	G4String idxstr = G4String(argv[ii+1]);
 	if(idxstr == "upper" || idxstr == "UPPER" ) {
	  ScoreData::theIndexValType = SIVT_UPPER;
	} else if(idxstr == "middle" || idxstr == "MIDDLE" ) {
	  ScoreData::theIndexValType = SIVT_MIDDLE;
	} else if(idxstr == "lower" || idxstr == "LOWER" ) {
	  ScoreData::theIndexValType = SIVT_LOWER;
	} else {
	  G4cerr << " INDEX VAL TYPE " << idxstr << G4endl;
	  G4Exception("sumScores",
		      "Error",
		      FatalErrorInArgument,
		      (G4String("Wrong index val type, it can be upper/UPPER, middle/MIDDLE or lower/LOWER, while it is") + idxstr).c_str());
	}
      } else {
	PrintHelp();
	G4Exception("compareScores",
		    "Error",
		    FatalErrorInArgument,
		    (G4String("ARGUMENT: ") + argvstr).c_str());
      }
    }
  }

  if( theCompareFileName == "" ) {
    G4Exception("compareScores",
		"Error",
		FatalException,
		"You have  not asked for a file, please use -f"); 
  }

  ReadCompareFile( theCompareFileName );

  std::set<pss>::const_iterator itef;
  for( itef = theFiles.begin(); itef != theFiles.end(); itef++ ) {
    if( ScoreData::verbosity >= 0) G4cout << "@@@@@@@ Reading FILE: " << (*itef).first << " of type " << (*itef).second << G4endl;
    ReadScores( (*itef).first, (*itef).second );
  }

  std::vector<ScoreSet*>::const_iterator itess;
  for( itess = theScoreSets.begin(); itess != theScoreSets.end(); itess++ )  {
    PrintData(*itess);
  }

  return 0;
}


//-------------------------------------------------------------------
void PrintHelp()
{
  G4cout << " -f          scores compare file name " << G4endl
	 << " -hXMin      Minimum X value of the histograms " << G4endl
	 << " -hXMax      Maximum X value of the histograms " << G4endl
	 << " -hYMin      Minimum Y value of the histograms " << G4endl
	 << " -hYMax      Maximum Y value of the histograms " << G4endl
	 << " -hLogX      Histogram X axis in logarithmic scale (1) or linear (0)" << G4endl
	 << " -hLogY      Histogram Y axis in logarithmic scale (1) or linear (0)" << G4endl
	 << " -verb       verbosity: sets the ShieldingVerbosity" << G4endl
	 << " -indexValType  Type of treatment of score index (get upper limit, lower limit or average " << G4endl
	 << " -help       prints the set of arguments " << G4endl;
}

//-----------------------------------------------------------------------------
void ReadCompareFile( G4String fileName )
{  
  std::vector<G4String> wl;
  GmFileIn fin = GmFileIn::GetInstance(fileName);
  ScoreSet* scoreSet = 0;
  for( G4int ii = 0;; ii++) {
    if( ! fin.GetWordsInLine(wl) ) break;
    if( wl[0] == "ADD" ) {
      scoreSet = new ScoreSet(wl);
      theScoreSets.push_back(scoreSet);
    } else {
      if( scoreSet == 0 ) {
	G4Exception("ReadCompareFile",
		    "",
		    FatalException,
		    "First line in file must start with ' ADD '");
      }
      G4String fileName;
      G4String scorerName;
      G4String fileType;
      G4double weight;
      G4bool bLineOK = false;
      if( wl.size() == 3 ) {
	fileName = wl[0];
	scorerName = wl[1];
	if( GmGenUtils::IsNumber(wl[2]) ) {
	  weight = GmGenUtils::GetValue(wl[2]);
	  bLineOK = true;
	}
	fileType = "GAMOS";
      } else if( wl.size() == 4 ) {
	fileName = wl[0];
	scorerName = wl[1];
	fileType = wl[2];
	//if( GmGenUtils::IsNumber(wl[3]) ) {
	  weight = GmGenUtils::GetValue(wl[3]);
	  bLineOK = true;
	 
      } 
      if( !bLineOK )  {
	G4cout << "READING LINE";
	for( size_t ii = 0; ii < wl.size(); ii++ ) {
	  G4cout << " " << wl[ii];
	}
	G4cout << G4endl;
	G4Exception("ReadCompareFile",
		    "",
		    FatalException,
		    (G4String("Lines line must have two or three words: FILE_NAME SCORE_NAME (FILE_TYPE) WEIGHT").c_str()));
      }
      theFiles.insert(pss(fileName,fileType));
      scoreSet->AddScorer(fileName,scorerName,weight);
    }
  }
}

//-----------------------------------------------------------------------------
void PrintData( ScoreSet* scoreSet ) 
{
  ScorerData* scorerDataSUM = new GAMOSScorerData();
  GAMOSScorerData* scorerDataSUMG = static_cast<GAMOSScorerData*>(scorerDataSUM);
  scorerDataSUMG->SetDetectorName(scoreSet->GetDetectorName());
  scorerDataSUMG->SetScorerName(scoreSet->GetScorerName());

  vpss fScorerNames = scoreSet->GetFileScorerNames();
  vpss::const_iterator itess;
  std::vector<G4double> fScoreWeights = scoreSet->GetScoreWeights();
  std::vector<G4double>::const_iterator ited;

  //--- add data
  G4double weight;
  for( itess = fScorerNames.begin(), ited = fScoreWeights.begin(); itess != fScorerNames.end(); itess++, ited++ ) {
    ScorerData* scorerData = FindScorerData( (*itess).first, (*itess).second );
    //-    G4cout << " SCORER DATA " << scorerData << G4endl;
    if( scorerData ) {
      weight = (*ited);
      (*scorerData) *= weight;
      if( itess == fScorerNames.begin() ) {
	(*scorerDataSUM) = (*scorerData);
      } else {
	(*scorerDataSUM) += (*scorerData);
	(*scorerDataSUM) *= 2.; //SUM gets average: multiplying by 2 is OK if nevents is the same in bothe scores
      }
    }
  }

  // WRITE score SUM in a file
  //  if( scoreSet->GetFileType() == "GAMOS" ){
  GAMOSFileData* fdSUM = new GAMOSFileData();
  fdSUM->AddScorerData(scorerDataSUM);
  G4cout << " @@@@@@@@@@@@@@@@@@@@@ scorerDataSUM " << scorerDataSUM << G4endl;
  G4cout << " GetNumberOfScorers() " << fdSUM->GetNumberOfScorers() << G4endl;
  //-  G4cout << "  GetScorerData " << fdSUM->GetScorerData(0) << G4endl;
  

  std::ofstream fout(scoreSet->GetOutFileName());
  fdSUM->Print(fout);
  /*  } else {
    G4Exception("addScoresWeighted",
		"",
		FatalException,
		"NO addScoresWeighted for MCNP scores, please ask GAMOS developers");
		}*/

}

//-----------------------------------------------------------------------------
ScorerData* FindScorerData( G4String fileName , G4String scorerName )
{
  ScorerData* scorerData = 0;

  std::vector<FileData*>::const_iterator itefd; 
  for( itefd = theFileData.begin(); itefd != theFileData.end(); itefd++ ) {
    FileData* fileData = (*itefd);
    if( fileData->GetFileName() == fileName ) {
      size_t nsco = fileData->GetNumberOfScorers();
      for( size_t ii = 0; ii < nsco; ii++ ) {
	ScorerData* sd = fileData->GetScorerData(ii);
	if( sd->GetName() == scorerName ) {
	  scorerData = sd;
	  break;
	}
      }
    }
  }
  if( scorerData == 0 ) {
    G4Exception("FindScorerData",
		"",
		JustWarning,
		(G4String("Scorer not found " + scorerName + " in file " + fileName).c_str()));
    G4cerr << "@@ LOOKING FOR FILE: " << fileName << " SCORER: " << scorerName << G4endl;
    for( itefd = theFileData.begin(); itefd != theFileData.end(); itefd++ ) {
      FileData* fileData = (*itefd);
      size_t nsco = fileData->GetNumberOfScorers();
      for( size_t ii = 0; ii < nsco; ii++ ) {
	ScorerData* sd = fileData->GetScorerData(ii);
	G4cerr << " FILE: " << fileData->GetFileName() << " SCORER: " << sd->GetName() << G4endl;
      }
    }
  } else {
  
    if( ScoreData::verbosity >= 2) G4cout << " FindScorerData " << fileName << " " << scorerName << " =N " << scorerData->GetNScores() << G4endl;
  }
  return scorerData;
}

