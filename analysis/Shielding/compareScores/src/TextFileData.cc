#include "TextFileData.hh"
#include "TextScorerData.hh"
#include "TextScoreData.hh"

#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include <set>

//-----------------------------------------------------------------------------
TextFileData::TextFileData(G4String fileName, G4bool bInvX)
{

  theFileName = fileName; 
  
  GmFileIn Infile = GmFileIn::GetInstance(fileName);
  Infile.SetSuppressQuotes(0);
  
  TextScorerData* doserData = 0;   	
  
  G4int jj;
  G4double nevents = 1;
  G4bool bDoserFound = false;
  std::vector<G4String> wl;
  for( jj = 0;; jj++) {
    if( ! Infile.GetWordsInLine(wl) ) break;    
    //----- Read and save the name of each  in the file    
    if ( wl.size() >= 2 && wl[0]=="DATA:" )   {
      nevents = 1;
      //	      G4cout << " Size wl " << wl.size() << G4endl;  
      bDoserFound = true;
      if( ScoreData::verbosity >= 1 ) G4cout << "@@@ New ScorerData DOSER    " <<  wl[1] << G4endl; 
      if( doserData && bInvX ) doserData->InvertX();

      doserData = new TextScorerData();
      AddScorerData( doserData );
      doserData->SetName( wl[1] );
      
      //---- Read and save dose
    } else if (wl.size() >= 2) {
      TextScoreData* doseData = new TextScoreData(wl);
      doserData->AddScoreData( doseData );
      doserData->SetNDoses( doserData->GetNScores() + 1 );
      if( ScoreData::verbosity >= 2 ) G4cout << "@@@ New ScoreData N ENTRIES " << doserData->GetNScores() << G4endl; 
      
    }
  }
  if( doserData && bInvX ) doserData->InvertX();
  

  Infile.Close();
  if( nevents == -1 && bDoserFound ) {
    G4Exception("compareDoses",
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


