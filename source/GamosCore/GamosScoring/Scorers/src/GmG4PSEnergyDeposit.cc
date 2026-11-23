#include "GmG4PSEnergyDeposit.hh"
#include "G4UnitsTable.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"
////////////////////////////////////////////////////////////////////////////////
// Description:
//   This is a primitive scorer class for scoring energy deposit.
///////////////////////////////////////////////////////////////////////////////

GmG4PSEnergyDeposit::GmG4PSEnergyDeposit(G4String name)
     :GmVPrimitiveScorer(name)
{
  theUnit = 1.;
  theUnitName = G4String("MeV");
}

GmG4PSEnergyDeposit::~GmG4PSEnergyDeposit()
{;}

G4bool GmG4PSEnergyDeposit::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  //  G4cout << " 1GmG4PSEnergyDeposit::ProcessHits( " << G4endl; //GDEB
  if( aStep == 0 ) return FALSE;  // it is 0 when called by GmScoringMgr after last event
  //  G4cout << " 2GmG4PSEnergyDeposit::ProcessHits( " << G4endl; //GDEB

  if( !AcceptByFilter( aStep ) ) return false;
  //  G4cout << " 3GmG4PSEnergyDeposit::ProcessHits( " << G4endl; //GDEB

  G4double edep = aStep->GetTotalEnergyDeposit();
  if ( edep == 0. ) return FALSE;
  G4double weight = aStep->GetPreStepPoint()->GetWeight(); 
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) 
    G4cout << "  GmG4PSEnergyDeposit::ProcessHits edep " << edep 
	   << G4endl;
#endif

 // use index from classifier, unless you are skipping borders of equal material voxels in G4RegularNavigation
  G4int index = GetIndex(aStep);
  FillScorer( aStep, index, edep, weight );
  
  return TRUE;
} 


