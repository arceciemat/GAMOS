#include "GmPSElectronicELost.hh"
#include "G4UnitsTable.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"
////////////////////////////////////////////////////////////////////////////////
// Description:
//   This is a primitive scorer class for scoring energy deposit.
///////////////////////////////////////////////////////////////////////////////

GmPSElectronicELost::GmPSElectronicELost(G4String name)
     :GmVPrimitiveScorer(name)
{
  theUnit = 1.;
  theUnitName = G4String("MeV");
}

GmPSElectronicELost::~GmPSElectronicELost()
{;}

G4bool GmPSElectronicELost::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  //  G4cout << " 1GmPSElectronicELost::ProcessHits( " << G4endl; //GDEB
  if( aStep == 0 ) return FALSE;  // it is 0 when called by GmScoringMgr after last event
  //  G4cout << " 2GmPSElectronicELost::ProcessHits( " << G4endl; //GDEB

  if( !AcceptByFilter( aStep ) ) return false;
  //  G4cout << " 3GmPSElectronicELost::ProcessHits( " << G4endl; //GDEB

  G4double edep = aStep->GetTotalEnergyDeposit();
  if ( edep == 0. ) return FALSE;
  G4double weight = aStep->GetPreStepPoint()->GetWeight(); 
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) 
    G4cout << "  GmPSElectronicELost::ProcessHits edep " << edep 
	   << G4endl;
#endif

 // use index from classifier, unless you are skipping borders of equal material voxels in G4RegularNavigation
  G4int index = GetIndex(aStep);
  FillScorer( aStep, index, edep, weight );
  
  return TRUE;
} 


