#include "GAMOSFileData.hh"
#include "GAMOSScorerData.hh"
#include "GAMOSScoreData.hh"

#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include <set>

//-----------------------------------------------------------------------------
GAMOSFileData::GAMOSFileData()
{
}
    
//-----------------------------------------------------------------------------
GAMOSFileData::GAMOSFileData(G4String fileName)
{

  theFileName = fileName; 
  
  GmFileIn Infile = GmFileIn::GetInstance(fileName,true);
  Infile.SetSuppressQuotes(0);
  
  GAMOSScorerData* scorerData = 0;   	
  
  G4int jj;
  G4double nevents = -1;
  G4bool bScorerFound = false;
  std::vector<G4String> wl;
  for( jj = 0;; jj++) {
    if( ! Infile.GetWordsInLine(wl) ) break;    
    //----- Read and save the name of each  in the file    
    if ( wl.size() > 2 && wl[0]=="Number" &&  wl[1]=="of"  &&  wl[2]=="events")   {
      nevents = GmGenUtils::GetValue (wl[5]);
      FileData::theNEventsSum += nevents;
      if( ScoreData::verbosity >= 1 ) G4cout << " NEvents " << nevents << G4endl; 
    }
    
    if(wl.size() > 4 && wl[0]=="Run" &&  wl[1]=="Aborted"  &&  wl[4]=="events") {
      nevents = GmGenUtils::GetValue (wl[3]);
      FileData::theNEventsSum += nevents;
      if( ScoreData::verbosity >= 1 ) G4cout << " NEvents " << nevents << G4endl; 
    }

    //    G4cout << " READ Wl0 " << wl[0] << G4endl; //GDEB
    if ( wl[0]=="MultiFunctionalDet:" )  {
      if( nevents == -1 ) {
	G4Exception("sumcores",
		    "No number of events",
		    FatalException,
		    (G4String("File ") + fileName + " has no number of events. Rerun it with '/run/verbose 1' user command").c_str());
      }
      
      //	      G4cout << " Size wl " << wl.size() << G4endl;  
      bScorerFound = true;
      scorerData = new GAMOSScorerData();
      scorerData->SetDetectorName( ExtractName(wl) );
      if( ScoreData::verbosity >= 1 ) G4cout << "@@@ New ScorerData DETECTOR  " <<  ExtractName(wl) << G4endl; 
      AddScorerData( scorerData );
      
    } else if ( wl[0]=="PrimitiveScorer:" ) {
      if( ! scorerData ) {
	G4Exception("GAMOSFileData::GAMOSFileData",
		    "",
		    FatalException,
		    "Reading 'PrimitiveScorer:'. File does not contain 'MultiFunctionalDet:' ");
      }
      scorerData->SetScorerName( ExtractName(wl) );
      scorerData->SetName( scorerData->GetName() );
      scorerData->SetNEvents( nevents ); // total sum is calculated for each type of scorer name
      if( ScoreData::verbosity >= 1 ) G4cout << "@@@ New ScorerData SCORER    " <<  ExtractName(wl) << " NEV " << scorerData->GetNEvents() << " = " << nevents << G4endl; 
    } else if (wl.size() == 4 && 
	       wl[0]=="Number" && 
	       wl[2] == "entries=") {
      if( ! scorerData ) {
	G4Exception("GAMOSFileData::GAMOSFileData",
		    "",
		    FatalException,
		    "Reading 'Number of entries'. File does not contain 'MultiFunctionalDet:' ");
      }
      scorerData->SetNScores( GmGenUtils::GetValue(wl[3]) );
      if( ScoreData::verbosity >= 1 ) G4cout << "@@@ New ScorerData N ENTRIES " << wl[3] << G4endl; 
      
      //---- Read and save score
    }  else if (wl.size() >= 6 && wl[0]=="index:") {
      if( ! scorerData ) {
	G4Exception("GAMOSFileData::GAMOSFileData",
		    "",
		    FatalException,
		    "Reading 'index:' File does not contain 'MultiFunctionalDet:' ");
      }
      GAMOSScoreData* scoreData = new GAMOSScoreData(wl, false);
      if( scoreData->SelfCheck() ) {
	scorerData->AddScoreData( scoreData);
      } else {
        scorerData->SetNScores( scorerData->GetNScores() - 1 );
      } 

    } else {
      for( unsigned int kk = 0; kk < wl.size()-1; kk++ ) {
	//	if (wl[kk]=="SUM" && wl[kk+1] == "ALL:") {
	if (wl[kk]=="SUM_ALL:") {
	  GAMOSScoreData* scoreData = new GAMOSScoreData(wl, true);
	  
	  scorerData->AddScoreData( scoreData);
	}
      }
    }
  }
  
  Infile.Close();
  if( nevents == -1 && bScorerFound ) {
    G4Exception("GAMOSFileData::GAMOSFileData",
		"Wrong argument",
		FatalErrorInArgument,
		"File does not contain number of events processed, it cannot be used");
  }


}


//-----------------------------------------------------------------------------
GAMOSFileData::GAMOSFileData(const FileData* fd2)
{
  const GAMOSFileData* gfd2 = static_cast<const GAMOSFileData*>(fd2);
  theFileName = gfd2->GetFileName();
  theScorerData = gfd2->GetAllScorerData();
}


//-----------------------------------------------------------------------------
void GAMOSFileData::Print( std::ofstream& fout )
{
  
  fout << "  Number of events processed : " << GAMOSFileData::theNEventsSum << G4endl;

  for( unsigned int ii = 0; ii < theScorerData.size(); ii++ )
    {
      theScorerData[ii]->Print(fout);
    }

}

//-------------------------------------------------------------------
G4String GAMOSFileData::ExtractName( std::vector<G4String> wl)
{
  G4String name= "";
  for( unsigned int ii = 1; ii < wl.size(); ii++ ){
    if( ii != 1 ) name += " ";
    name += wl[ii];
  }

  return name;
}


