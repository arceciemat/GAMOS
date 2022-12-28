#include "GmPSElectronicdEdx.hh"
#include "G4UnitsTable.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"
#include "G4EmCalculator.hh"


GmPSElectronicdEdx::GmPSElectronicdEdx(G4String name)
     :GmVPrimitiveScorer(name)
{
  theUnit = 1.;
  theUnitName = G4String("MeV/mm");
  theEmCalculator = new G4EmCalculator;
}

GmPSElectronicdEdx::~GmPSElectronicdEdx()
{;}

G4bool GmPSElectronicdEdx::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  if( aStep == 0 ) return FALSE;  // it is 0 when called by GmScoringMgr after last event

  if( !AcceptByFilter( aStep ) ) return false;

  G4double edep = aStep->GetTotalEnergyDeposit();
  if ( edep == 0. ) return FALSE;

  G4double dedx = GetElectronicDEDX( aStep, true);
  G4double weight = aStep->GetPreStepPoint()->GetWeight();
  
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) {
    G4cout << "  GmPSElectronicdEdx::ProcessHits dedx " << dedx
	   << G4endl;
}
#endif

  G4int index = GetIndex(aStep);
  FillScorer( aStep, index, dedx, weight );
  
  return TRUE;
} 

