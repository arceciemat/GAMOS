#include "MCNPTallierFNData.hh"
#include "MCNPTallyData.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"
#include "G4tgrUtils.hh"
#include "G4UImanager.hh"
#include "GamosCore/GamosData/Distributions/include/GmNumericDistributionLogLog.hh"
#include "GamosCore/GamosData/Distributions/include/GmNumericDistributionLinLog.hh"
#define private public
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

//------------------------------------------------------------------------
MCNPTallierFNData::MCNPTallierFNData( std::vector<G4String>& wl, GmFileIn& fin, G4bool bMod ) 
  : MCNPVTallierData(wl,bMod)
{

  theScoreType = ST_MCNP_FN;
  ReadData(wl, fin);

}

//------------------------------------------------------------------------
G4bool MCNPTallierFNData::CheckData( std::vector<G4String>& wl ) 
{
  if( wl[0] == "cell:" ) {
    G4tgrUtils::CheckWLsize( wl, 2, WLSIZE_EQ, " MCNPTallierFNData::MCNPTallierFNData");
    theCell = GmGenUtils::GetInteger(wl[1]);
    if( ScoreData::verbosity >= 2 ) G4cout << "MCNPFileData: read cell " << theCell << G4endl;
    return TRUE;
  } else {
    return FALSE;
  }
}

//----------------------------------------------------------------------
void MCNPTallierFNData::Print( std::ofstream& fout )
{
  fout << "1" << theName << "       nps = " << theNEvents << G4endl;
  fout << "          tally type " << theTallyType << "   number of particles crossing a surface.                 " << G4endl;
  fout << "          particle(s): " << theParticle << G4endl;
 
  fout << "surface  " << G4endl; 
  fout << "      energy " << G4endl;

  std::vector<ScoreData*>::const_iterator ite;
  for( ite = theScoreData.begin(); ite != theScoreData.end(); ite++ ) {
    (*ite)->Print(fout);
  }

}
