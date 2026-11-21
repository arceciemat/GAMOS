#include "CSVScoreData.hh"
#include "ScorerData.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//----------------------------------------------------------------------
CSVScoreData::CSVScoreData( G4double binCenter, G4double binContents, G4double binError )
  : ScoreData()
{
  theName = GmGenUtils::ftoa( binCenter );
  theDose = binContents;
  theDoseError = binError;
  theDoseSq = theDose*theDose;
  if( theDoseError != 0 ) {
    theDoseDivideError = theDose/(theDoseError*theDoseError);
    theInvErrorExp2 = 1./(theDoseError*theDoseError);
  } else {
    theDoseDivideError = 0;
    theInvErrorExp2 = 0;
  }
  theNumberOfFiles = 1;
  bTotal = false;
  theScorerData = 0;

  if( ScoreData::verbosity >= 2 ) {
    G4cout << " CSVScoreData " << theName << " : " << theDose << " +- " << theDoseError << G4endl;
  }
}

//----------------------------------------------------------------------
void CSVScoreData::Print( std::ofstream& fout )
{
  G4double errorRel = 0.;
  if( theDose != 0. ) errorRel = theDoseError/theDose;
  fout << " index: " << theName << "  = " << theDose << " +-(REL) " << errorRel << G4endl;
}
