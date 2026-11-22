#include "GmPSDose_LETBydEdx_Unrestricted.hh"
#include "G4UnitsTable.hh"
#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"
#include "G4EmCalculator.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"


GmPSDose_LETBydEdx_Unrestricted::GmPSDose_LETBydEdx_Unrestricted(G4String name)
     :GmVPrimitiveScorer(name)
{
  theUnit = 1.;
  theUnitName = G4String("MeV/mm");
  theEmCalculator = new G4EmCalculator;
}

GmPSDose_LETBydEdx_Unrestricted::~GmPSDose_LETBydEdx_Unrestricted()
{;}

G4bool GmPSDose_LETBydEdx_Unrestricted::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  if( aStep == 0 ) return FALSE;  // it is 0 when called by GmScoringMgr after last event

  if( !AcceptByFilter( aStep ) ) return false;

  G4double edep = aStep->GetTotalEnergyDeposit();
  if ( edep == 0. ) return FALSE;
  
  G4ParticleDefinition* particleDef = aStep->GetTrack()->GetDefinition();
  if( particleDef->GetPDGCharge() == 0 || particleDef == G4Electron::Electron() || particleDef == G4Positron::Positron() ) return FALSE;
      
  G4StepPoint* stepPoint = aStep->GetPreStepPoint();
  G4ProductionCuts* prodCuts = stepPoint->GetPhysicalVolume()->GetLogicalVolume()->GetRegion()->GetProductionCuts();
  G4ParticleDefinition* part =  aStep->GetTrack()->GetDefinition();
  G4double EKinAver = (stepPoint->GetKineticEnergy()+aStep->GetPostStepPoint()->GetKineticEnergy())/2.;
  G4double let = theEmCalculator->ComputeElectronicDEDX( EKinAver, 
				  part, 
				   stepPoint->GetMaterial(), 
						       prodCuts->GetProductionCut(part->GetParticleName()));

  const std::vector<const G4Track*> * secondary = aStep->GetSecondaryInCurrentStep();
  G4double elecELost = edep; // electronic energy lost
  for( size_t ii = 0; ii < secondary->size(); ii++ ) {
    const G4Track* secoTrk = secondary->at(ii);
    if( secoTrk->GetDefinition() == G4Electron::Electron() ) {
      elecELost += secoTrk->GetKineticEnergy();
    }
  }

  G4double eeLostLET = elecELost*let;
  G4double weight = aStep->GetPreStepPoint()->GetWeight(); 
#ifndef GAMOS_NO_VERBOSE
  if( ScoringVerb(debugVerb) ) {
    G4cout << "  GmPSDose_LETBydEdx_Unrestricted::ProcessHits eeLostLET " << eeLostLET
	   << " let " << let
	   << " edep " << edep
	   << G4endl;
    G4cout << aStep->GetTrack()->GetKineticEnergy() << "  GmPSDose_LETBydEdx_Unrestricted::ProcessHits let TEST " << (1.-let/(aStep->GetTotalEnergyDeposit()/aStep->GetStepLength())) << " " << let << " = " << aStep->GetTotalEnergyDeposit()/aStep->GetStepLength() << G4endl;
}
#endif

  G4int index = GetIndex(aStep);
  FillScorer( aStep, index, eeLostLET, weight );
  
  return TRUE;
} 

