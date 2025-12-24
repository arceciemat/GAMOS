#include "GmPSDose_LET_dEdx.hh"
#include "G4UnitsTable.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"
#include "G4EmCalculator.hh"
#include "G4ProcessType.hh"
#include "G4VProcess.hh"

GmPSDose_LET_dEdx::GmPSDose_LET_dEdx(G4String name)
     :GmVPrimitiveScorer(name)
{
  theUnit = 1.;
  theUnitName = G4String("MeV/mm");
  theEmCalculator = new G4EmCalculator;
}

GmPSDose_LET_dEdx::~GmPSDose_LET_dEdx()
{;}

G4bool GmPSDose_LET_dEdx::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  if( aStep == 0 ) return FALSE;  // it is 0 when called by GmScoringMgr after last event

  if( !AcceptByFilter( aStep ) ) return false;

  G4double edep = aStep->GetTotalEnergyDeposit();
  if ( edep == 0. ) return FALSE;

  G4double let = 0.;
  // G4ProcessType procType = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessType();
  //t  if( procType == fTransportation || procType == fElectromagnetic ) {
    let = GetElectronicDEDX( aStep, true );
    //t  }
  G4double doselet = edep*let;
  G4double weight = aStep->GetPreStepPoint()->GetWeight(); 
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) {
    G4cout << "  GmPSDose_LET_dEdx::ProcessHits doselet " << doselet
	   << " let " << let
	   << " edep " << edep
	   << G4endl;
    //    G4cout << aStep->GetTrack()->GetKineticEnergy() << "  GmPSDose_LET_dEdx::ProcessHits let TEST " << (1.-let/(aStep->GetTotalEnergyDeposit()/aStep->GetStepLength())) << " " << let << " = " << aStep->GetTotalEnergyDeposit()/aStep->GetStepLength() << G4endl;
}
#endif

  G4int index = GetIndex(aStep);
  FillScorer( aStep, index, doselet, weight );
  
  return TRUE;
} 

