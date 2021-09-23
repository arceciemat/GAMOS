#include "MCNPTallyData.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "G4tgrUtils.hh"
#include "G4UImanager.hh"
#include "GamosCore/GamosData/Distributions/include/GmNumericDistributionLogLog.hh"
#include "GamosCore/GamosData/Distributions/include/GmNumericDistributionLinLog.hh"
#define private public
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

//------------------------------------------------------------------------
MCNPTallyData::MCNPTallyData( const std::vector<G4String>& wl )
{
  G4tgrUtils::CheckWLsize( wl, 3, WLSIZE_GE, "  MCNPTallyData::MCNPTallyData");
  if( ScoreData::verbosity >= 2 ) G4cout << " AddTally " << wl[0] << " " << wl[1] << " " << wl[2] << G4endl;
  if( wl[0] == "total" ) {
    theName = "SUM_ALL:";
    theScore = GmGenUtils::GetValue(wl[1]);
    theScoreError = GmGenUtils::GetValue(wl[2]);
    //    theScoreTot = GmGenUtils::GetValue(wl[1]);
    //   theScoreTotError = GmGenUtils::GetValue(wl[2]);
  } else {
    theName = wl[0];
    theScore = GmGenUtils::GetValue(wl[1]);
    theScoreError = GmGenUtils::GetValue(wl[2])*theScore;
  if( ScoreData::verbosity >= 2 ) G4cout << " MCNPTallyData " << wl[0] << " " << wl[1] << " " << wl[2] << G4endl;
  }

  SetVariables();

}

//------------------------------------------------------------------------
MCNPTallyData::MCNPTallyData( G4String xVal, G4double tallyVal, G4double tallyErr )
{
  if( ScoreData::verbosity >= 2 ) G4cout << " AddTally " << xVal << " " << tallyVal << " " << tallyErr << G4endl;
  theName = xVal;
  theScore = tallyVal;
  theScoreError = tallyErr;

  SetVariables();
}

//------------------------------------------------------------------------
void MCNPTallyData::SetVariables()
{
  theScoreSq = theScore*theScore;
  
  if( theScoreError != 0 ) {  
    theScoreDivideError = theScore/sqr(theScoreError);
    theInvErrorExp2 = 1./sqr(theScoreError);
  } else {
    theScoreDivideError = 0;
    theInvErrorExp2 = 0;
  }
  
  //  if( theName.find( "neutron_7" )!= std::string::npos)
  if( ScoreData::verbosity >= 2 )
    G4cout << " MCNPTallyData " << theName << " : " << theScore << " +- " << theScoreError << " DivideError " << theScoreDivideError << " InvErrorExp2 " << theInvErrorExp2 << G4endl;
  
  theNumberOfFiles = 1;
  
}

//----------------------------------------------------------------------
void MCNPTallyData::Print( std::ofstream& fout )
{
G4String name = theName;
if( name == "SUM_ALL:" ) name = "total";
  fout << "   " << name << "  " << theScore << " " << theScoreError << G4endl;
}
