#include "CSVScorerData.hh"
#include "CSVScoreData.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//----------------------------------------------------------------------
CSVScorerData::CSVScorerData( const G4String& name, G4int nBins, G4double xMin, G4double xMax,
			      const std::vector<G4double>& contents, const std::vector<G4double>& errors )
  : ScorerData()
{
  theDoseType = ST_CSV;
  theName = name;
  theNumberOfFiles = 1;
  G4double binWidth = (xMax-xMin)/nBins;
  for( G4int ii = 1; ii <= nBins; ii++ ) {
    CSVScoreData* doseData = new CSVScoreData( xMin+(ii-0.5)*binWidth, contents[ii], errors[ii] );
    AddScoreData( doseData );
    SetNDoses( GetNScores() + 1 );
  }
  if( ScoreData::verbosity >= 2 ) G4cout << "@@@ New CSVScorerData " << theName << " N SCORES " << GetNScores() << G4endl;
}

//----------------------------------------------------------------------
void CSVScorerData::Print( std::ofstream& fout )
{
  fout << " Doser: " << theName << G4endl;
  fout << " Number of entries= " << theNDoses << G4endl;
  for( size_t ii = 0; ii < theScoreData.size(); ii++ ) {
    theScoreData[ii]->Print(fout);
  }
}

//----------------------------------------------------------------------
G4double CSVScorerData::GetIndexVal( G4String doseName, G4bool )
{
  return GmGenUtils::GetValue(doseName);
}
