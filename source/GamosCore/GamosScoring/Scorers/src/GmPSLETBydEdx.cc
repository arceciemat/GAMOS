#include "GmPSLETBydEdx.hh"
#include "G4UnitsTable.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"
#include "G4EmCalculator.hh"


GmPSLETBydEdx::GmPSLETBydEdx(G4String name)
     :GmVPrimitiveScorer(name)
{
  theUnit = 1.;
  theUnitName = G4String("MeV/mm");
  theEmCalculator = new G4EmCalculator;
}

GmPSLETBydEdx::~GmPSLETBydEdx()
{;}

G4bool GmPSLETBydEdx::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  if( aStep == 0 ) return FALSE;  // it is 0 when called by GmScoringMgr after last event

  if( !AcceptByFilter( aStep ) ) return false;

  G4double edep = aStep->GetTotalEnergyDeposit();
  if ( edep == 0. ) return FALSE;

  G4StepPoint* stepPoint = aStep->GetPreStepPoint();
  G4ProductionCuts* prodCuts = stepPoint->GetPhysicalVolume()->GetLogicalVolume()->GetRegion()->GetProductionCuts();
  G4ParticleDefinition* part =  aStep->GetTrack()->GetDefinition();
  G4double EKinAver = (stepPoint->GetKineticEnergy()+aStep->GetPostStepPoint()->GetKineticEnergy())/2.;
  G4double let = theEmCalculator->ComputeElectronicDEDX( EKinAver, 
							 part, 
							 stepPoint->GetMaterial(), 
							 prodCuts->GetProductionCut(part->GetParticleName()));
   
  G4double weight = aStep->GetPreStepPoint()->GetWeight(); 
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) {
    G4cout << "  GmPSLETBydEdx::ProcessHits let " << let	 
	   << G4endl;
    G4cout << aStep->GetTrack()->GetKineticEnergy() << "  GmPSLETBydEdx::ProcessHits let TEST " << (1.-let/(aStep->GetTotalEnergyDeposit()/aStep->GetStepLength())) << " " << let << " = " << aStep->GetTotalEnergyDeposit()/aStep->GetStepLength() << G4endl;
}
#endif

  G4int index = GetIndex(aStep);
  FillScorer( aStep, index, let, weight );
  
  return TRUE;
} 

