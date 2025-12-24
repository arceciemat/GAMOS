#include "GmPSLET.hh"
#include "G4UnitsTable.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"

GmPSLET::GmPSLET(G4String name)
     :GmVPrimitiveScorer(name)
{
  theUnit = 1.;
  theUnitName = G4String("MeV/mm");
}

GmPSLET::~GmPSLET()
{;}

G4bool GmPSLET::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  if( aStep == 0 ) return FALSE;  // it is 0 when called by GmScoringMgr after last event

  if( !AcceptByFilter( aStep ) ) return false;

  G4double edep = aStep->GetTotalEnergyDeposit();
  if ( edep == 0. ) return FALSE;
  G4double length = aStep->GetStepLength();
  G4double let;
  if( length > 1.e-100 ) {
    let = edep/length;
  } else {
    let = 0.;
  } 
  G4double weight = aStep->GetPreStepPoint()->GetWeight(); 
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) 
    G4cout << "  GmPSLET::ProcessHits let " << let	 
	   << " edep " << edep
	   << " / length " << length
	   << G4endl;
#endif

  G4int index = GetIndex(aStep);
  FillScorer( aStep, index, let, weight );
  
  return TRUE;
} 

