#include "GmPSLET_EDep.hh"
#include "G4UnitsTable.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"

GmPSLET_EDep::GmPSLET_EDep(G4String name)
     :GmVPrimitiveScorer(name)
{
  theUnit = 1.;
  theUnitName = G4String("MeV/mm");
}

GmPSLET_EDep::~GmPSLET_EDep()
{;}

G4bool GmPSLET_EDep::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  if( aStep == 0 ) return FALSE;  // it is 0 when called by GmScoringMgr after last event

  if( !AcceptByFilter( aStep ) ) return false;

  G4double edep = aStep->GetTotalEnergyDeposit();
  if ( edep == 0. ) return FALSE;
  G4double length = aStep->GetStepLength();
  G4double let = 0.;
  if( length > 1.e-100 ) {
    let = edep/length;
  } 
  G4double weight = aStep->GetPreStepPoint()->GetWeight(); 
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) 
    G4cout << "  GmPSLET_EDep::ProcessHits let " << let	 
	   << " edep " << edep
	   << " / length " << length
	   << G4endl;
#endif

  G4int index = GetIndex(aStep);
  FillScorer( aStep, index, let, weight );
  
  return TRUE;
} 

