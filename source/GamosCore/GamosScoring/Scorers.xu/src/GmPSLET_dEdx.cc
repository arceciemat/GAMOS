#include "GmPSLET_dEdx.hh"
#include "GmComputeDEDX.hh"
#include "G4UnitsTable.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"
#include "G4EmCalculator.hh"
#include "G4ProcessType.hh"
#include "G4VProcess.hh"

GmPSLET_dEdx::GmPSLET_dEdx(G4String name)
     :GmVPrimitiveScorer(name)
{
  theUnit = 1.;
  theUnitName = G4String("MeV/mm");
  theEmCalculator = new G4EmCalculator;
}

GmPSLET_dEdx::~GmPSLET_dEdx()
{;}

G4bool GmPSLET_dEdx::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  if( aStep == 0 ) return FALSE;  // it is 0 when called by GmScoringMgr after last event

  if( !AcceptByFilter( aStep ) ) return false;

  G4double edep = aStep->GetTotalEnergyDeposit();
  if ( edep == 0. ) return FALSE;

  // G4cout << " LET_R " << GetdEdx( aStep, true) << G4endl; //GDEB
  //  G4cout << " LET_U " << GetdEdx( aStep, false) << G4endl; //GDEB
  //  G4double let = GetdEdx( aStep, true);

  G4double let = 0.;
  G4ProcessType procType = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessType();
  if( procType == fTransportation || procType == fElectromagnetic ) {
    let = GetElectronicDEDX( aStep, true );
  }
  
  G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
  G4double weight = preStepPoint->GetWeight(); 
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) {
    G4ParticleDefinition* part =  aStep->GetTrack()->GetDefinition();
    G4cout << "  GmPSLET_dEdx::ProcessHits let " << let
      //	   << " letGet " << GetDEDX( aStep, true) 
	   << " edep " << edep
	   << " Ekin " << preStepPoint->GetKineticEnergy()
	   << " mate " << preStepPoint->GetMaterial()->GetName()
	   << " part " << part->GetParticleName() 
	   << G4endl;
    //    G4cout << aStep->GetTrack()->GetKineticEnergy() << "  GmPSDose_LET_dEdx::ProcessHits let TEST " << (1.-let/(aStep->GetTotalEnergyDeposit()/aStep->GetStepLength())) << " " << let << " = " << aStep->GetTotalEnergyDeposit()/aStep->GetStepLength() << G4endl;
}
#endif

  G4int index = GetIndex(aStep);
  FillScorer( aStep, index, let, weight );
  
  return true;
} 

