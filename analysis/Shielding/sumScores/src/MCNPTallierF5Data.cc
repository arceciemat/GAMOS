#include "MCNPTallierF5Data.hh"
#include "MCNPTallyData.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "G4tgrUtils.hh"
#include "G4UImanager.hh"
#include "GamosCore/GamosData/Distributions/include/GmNumericDistributionLogLog.hh"
#include "GamosCore/GamosData/Distributions/include/GmNumericDistributionLinLog.hh"
#define private public
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

//------------------------------------------------------------------------
MCNPTallierF5Data::MCNPTallierF5Data( std::vector<G4String>& wl, GmFileIn& fin, G4bool bMod )
  : MCNPVTallierData(wl,bMod)
{
  theScoreType = ST_MCNP_F5;
  ReadData(wl,fin);
}

//------------------------------------------------------------------------
G4bool MCNPTallierF5Data::CheckData( std::vector<G4String>& wl ) 
{
  if( wl[0] == "detector" && wl[1] == "located" ) {
    G4tgrUtils::CheckWLsize( wl, 5, WLSIZE_GE, " MCNPTallierF5Data::MCNPTallierF5Data");
    unsigned int wlN = wl.size();
    G4String posStr = wl[wlN-3] + wl[wlN-2] + wl[wlN-1];
    //	G4cout << " POSSTR " << posStr << G4endl;
    size_t iEold = posStr.find("=");
    std::vector<G4String> posV;
    for( int ii = 0; ii < 3; ii++ ){
      size_t iE = posStr.find("E",iEold+1);
      //	  G4cout << " iE " << iE << " iEold " << iEold << G4endl;
      G4String pos1 = (posStr.substr(iEold+1,iE-iEold+3));
      posV.push_back(pos1);
      //	  G4cout << " POSV " << ii << " " << pos1 << " iE " << iE << " iEold " << iEold << G4endl;
      iEold = iE+3;
    }
    thePosition = G4ThreeVector( GmGenUtils::GetValue(posV[0]) , GmGenUtils::GetValue(posV[1]), GmGenUtils::GetValue(posV[2]));
    if( ScoreData::verbosity >= 2 ) G4cout << "MCNPTallierF5Data: read position " << thePosition << G4endl;
    return TRUE;
  } else {
    return FALSE;
  }
}

//------------------------------------------------------------------------
void MCNPTallierF5Data::Print( std::ofstream& fout )
{
  fout << "1" << theName << "       nps = " << theNEvents << G4endl;
  fout << "          tally type " << theTallyType << "   particle flux at a point detector.           units   1/cm**2  " << G4endl;
  fout << "          particle(s): " << theParticle << G4endl;
  fout << G4endl;
  fout << " detector located at x,y,z = " << thePosition.x() << " " << thePosition.y() << " " << thePosition.z() << G4endl;
  fout << "      energy " << G4endl;

  std::vector<ScoreData*>::const_iterator ite;
  for( ite = theScoreData.begin(); ite != theScoreData.end(); ite++ ) {
    (*ite)->Print(fout);
  }

}
/*
//------------------------------------------------------------------------
MCNPTallierF5Data::MCNPTallierF5Data( const MCNPTallierF5Data& tallyN )
  : MCNPVTallierData(tallyN.IsModified())
{
  thePosition = tallyN.GetPosition();
}

*/
