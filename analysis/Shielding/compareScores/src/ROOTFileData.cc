#include "ROOTFileData.hh"
#include "ROOTScorerData.hh"
#include "ROOTScoreData.hh"

#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "TFile.h"
#include "TH1F.h"
#include "TKey.h"

//-----------------------------------------------------------------------------
ROOTFileData::ROOTFileData(G4String fileName, G4bool bInvX)
{
  theFileName = fileName; 
  
  TFile* file = new TFile(fileName);

  TIter ListOfHistograms( file->GetListOfKeys() );
  TKey *HistoKey;
  TH1F *histo1;
 
  ROOTScorerData* doserData = 0;   	
  while ( (HistoKey = (TKey*)ListOfHistograms()) ) { 
    std::string hisname = HistoKey->GetName()+std::string(";1");
    //ONLY HISTOGRAMS THAT CONTAIN 'NAME'     if( hisname.find("1E4") == std::string::npos ) continue;
    //ONLY HISTOGRAMS THAT DO NOT CONTAIN 'NAME'     if( hisname.find("NAME") != std::string::npos ) continue;

    TH1* histo = (TH1*)file->Get( HistoKey->GetName() );
    if( histo->GetDimension() == 1 ){
      if( dynamic_cast<TH1F*>(histo) ) {
	histo1 = (TH1F*) file->Get( HistoKey->GetName() );	
	//	std::cout << " comparing " << histo->GetName() << std::endl; //GDEB

	if( ScoreData::verbosity >= 1 ) G4cout << "@@@ New ScorerData DOSER    " <<  HistoKey->GetName() << G4endl; 
	if( doserData && bInvX ) doserData->InvertX();
	doserData = new ROOTScorerData(histo1);
	AddScorerData( doserData );
	doserData->SetName( histo->GetName() );
     
      }
    }
  }
  if( doserData && bInvX ) doserData->InvertX();

}


//-----------------------------------------------------------------------------
ROOTFileData::ROOTFileData(const FileData* fd2)
{
  const ROOTFileData* gfd2 = static_cast<const ROOTFileData*>(fd2);
  theFileName = gfd2->GetFileName();
  theScorerData = gfd2->GetAllScorerData();
}


//-----------------------------------------------------------------------------
void ROOTFileData::Print( std::ofstream& fout )
{
  
  fout << "  Number of events processed : " << ROOTFileData::theNEventsSum << G4endl;

  for( unsigned int ii = 0; ii < theScorerData.size(); ii++ )
    {
      theScorerData[ii]->Print(fout);
    }

}

//-------------------------------------------------------------------
G4String ROOTFileData::ExtractName( std::vector<G4String> wl)
{
  G4String name= "";
  for( unsigned int ii = 1; ii < wl.size(); ii++ ){
    if( ii != 1 ) name += " ";
    name += wl[ii];
  }

  return name;
}


