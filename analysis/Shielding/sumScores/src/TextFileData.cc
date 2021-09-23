#include "TextFileData.hh"
#include "TextScorerData.hh"
#include "TextScoreData.hh"

#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include <set>

//-----------------------------------------------------------------------------
TextFileData::TextFileData(G4String fileName)
{

  theFileName = fileName; 
  
  GmFileIn Infile = GmFileIn::GetInstance(fileName);
  Infile.SetSuppressQuotes(0);
  
  TextScorerData* scorerData = 0;   	
  
  G4int jj;
  G4double nevents = 1;
  G4bool bScorerFound = false;
  std::vector<G4String> wl;
  for( jj = 0;; jj++) {
    if( ! Infile.GetWordsInLine(wl) ) break;    
    //----- Read and save the name of each  in the file    
    if ( wl.size() >= 2 && wl[0]=="DATA:" )   {
      nevents = 1;
      FileData::theNEventsSum += nevents;    
      //	      G4cout << " Size wl " << wl.size() << G4endl;  
      bScorerFound = true;
      scorerData = new TextScorerData();
      //      scorerData->SetDetectorName( ExtractName(wl) );
      scorerData->SetDetectorName("");
      if( ScoreData::verbosity >= 1 ) G4cout << "@@@ New ScorerData DETECTOR  " <<  ExtractName(wl) << G4endl; 
      AddScorerData( scorerData );
      scorerData->SetScorerName( ExtractName(wl) );
      scorerData->SetName( scorerData->GetName() );
      scorerData->SetNEvents( G4int(nevents) ); // total sum is calculated for each type of scorer name
      if( ScoreData::verbosity >= 1 ) G4cout << "@@@ New ScorerData SCORER    " <<  ExtractName(wl) << G4endl; 

      //t      scorerData->SetNScores( GmGenUtils::GetValue(wl[3]) );
      
      //---- Read and save score
    }  else if (wl.size() >= 2) {
      TextScoreData* scoreData = new TextScoreData(wl);
      
      scorerData->AddScoreData( scoreData);
      scorerData->SetNScores( scorerData->GetNScores() + 1 );
      if( ScoreData::verbosity >= 2 ) G4cout << "@@@ New ScorerData N ENTRIES " << scorerData->GetNScores() << G4endl; 
      
    }
  }
  
  Infile.Close();
  if( nevents == -1 && bScorerFound ) {
    G4Exception("compareScores",
		"Wrong argument",
		FatalErrorInArgument,
		"File does not contain number of events processed, it cannot be used");
  }


}


//-----------------------------------------------------------------------------
TextFileData::TextFileData(const FileData* fd2)
{
  const TextFileData* gfd2 = static_cast<const TextFileData*>(fd2);
  theFileName = gfd2->GetFileName();
  theScorerData = gfd2->GetAllScorerData();
}


//-----------------------------------------------------------------------------
void TextFileData::Print( std::ofstream& fout )
{
  
  fout << "  Number of events processed : " << TextFileData::theNEventsSum << G4endl;

  for( unsigned int ii = 0; ii < theScorerData.size(); ii++ )
    {
      theScorerData[ii]->Print(fout);
    }

}

//-------------------------------------------------------------------
G4String TextFileData::ExtractName( std::vector<G4String> wl)
{
  G4String name= "";
  for( unsigned int ii = 1; ii < wl.size(); ii++ ){
    if( ii != 1 ) name += " ";
    name += wl[ii];
  }

  return name;
}


