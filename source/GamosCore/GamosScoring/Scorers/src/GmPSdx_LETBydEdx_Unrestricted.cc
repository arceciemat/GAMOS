#include "GmPSdx_LETBydEdx_Unrestricted.hh"
#include "G4UnitsTable.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"
#include "G4EmCalculator.hh"


GmPSdx_LETBydEdx_Unrestricted::GmPSdx_LETBydEdx_Unrestricted(G4String name)
     :GmVPrimitiveScorer(name)
{
  theUnit = 1.;
  theUnitName = G4String("MeV");
  theEmCalculator = new G4EmCalculator;
}

GmPSdx_LETBydEdx_Unrestricted::~GmPSdx_LETBydEdx_Unrestricted()
{;}

G4bool GmPSdx_LETBydEdx_Unrestricted::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  if( aStep == 0 ) return FALSE;  // it is 0 when called by GmScoringMgr after last event

  if( !AcceptByFilter( aStep ) ) return false;

  G4double edep = aStep->GetTotalEnergyDeposit();
  if ( edep == 0. ) return FALSE;

  G4ParticleDefinition* particleDef = aStep->GetTrack()->GetDefinition();
  
  G4StepPoint* stepPoint = aStep->GetPreStepPoint();
  G4ProductionCuts* prodCuts = stepPoint->GetPhysicalVolume()->GetLogicalVolume()->GetRegion()->GetProductionCuts();
  G4ParticleDefinition* part =  aStep->GetTrack()->GetDefinition();
  G4double EKinAver = (stepPoint->GetKineticEnergy()+aStep->GetPostStepPoint()->GetKineticEnergy())/2.;
  G4double let = theEmCalculator->ComputeElectronicDEDX( EKinAver,
							 part, 
							 stepPoint->GetMaterial(), 
							 prodCuts->GetProductionCut(part->GetParticleName()));

  G4double stepLen = aStep->GetStepLength();
  G4double dxlet = stepLen*let;
  
  G4double weight = aStep->GetPreStepPoint()->GetWeight(); 
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) {
    G4cout << "  GmPSdx_LETBydEdx_Unrestricted::ProcessHits dxlet " << dxlet
	   << " let " << let
	   << " stepLength " << stepLen
	   << G4endl;
    G4cout << aStep->GetTrack()->GetKineticEnergy() << "  GmPSdx_LETBydEdx_Unrestricted::ProcessHits let TEST " << (1.-let/(aStep->GetTotalEnergyDeposit()/aStep->GetStepLength())) << " " << let << " = " << aStep->GetTotalEnergyDeposit()/aStep->GetStepLength() << G4endl;
  }
#endif
  
  G4int index = GetIndex(aStep);
  FillScorer( aStep, index, dxlet, weight );
  
  return TRUE;
} 

