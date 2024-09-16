#include "GmDataInitialStoppingPower.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4VPhysicalVolume.hh"
#include "G4TouchableHistory.hh"
#include "G4TransportationManager.hh"
#include "G4PVParameterised.hh"

//----------------------------------------------------------------
GmDataInitialStoppingPower::GmDataInitialStoppingPower()
{
  bInitial = true;
  theHMin = 0.;
  theHMax = 10.;

}

//----------------------------------------------------------------
GmDataInitialStoppingPower::~GmDataInitialStoppingPower()
{
}


//----------------------------------------------------------------
G4double GmDataInitialStoppingPower::GetValueFromStep( const G4Step* aStep, G4int )
{
  G4StepPoint* preStep = aStep->GetPreStepPoint();
  /*  G4double dEdx1 = theEMCalculator.GetDEDX(preStep->GetKineticEnergy(),
					  aStep->GetTrack()->GetParticleDefinition(),
					  preStep->GetMaterial(),
					  preStep->GetPhysicalVolume()->GetLogicalVolume()->GetRegion());

  G4double cutInRange = preStep->GetMaterialCutsCouple()->GetProductionCuts()->GetProductionCut( aStep->GetTrack()->GetParticleDefinition()->GetParticleName());
  //G4double cut =  theEMCalculator.ComputeEnergyCutFromRangeCut(cutInRange, aStep->GetTrack()->GetParticleDefinition(),preStep->GetMaterial());
  G4double cut =  theEMCalculator.ComputeEnergyCutFromRangeCut(0.700, aStep->GetTrack()->GetParticleDefinition(),preStep->GetMaterial());

  cut = 277.633/1000.; // 0.7 in water e-
  G4double dEdx2 = theEMCalculator.ComputeTotalDEDX(preStep->GetKineticEnergy(),
						    aStep->GetTrack()->GetParticleDefinition(),
						    preStep->GetMaterial(),
						    cut);
  */
  G4double dEdx = theEMCalculator.ComputeTotalDEDX(preStep->GetKineticEnergy(),
						   aStep->GetTrack()->GetParticleDefinition(),
						   preStep->GetMaterial(),
						   DBL_MAX);
  //  G4cout << " GmDataInitialStoppingPower::GetValueFromStep dEdx= " << dEdx	 << " dEdx.GetDEDX " << dEdx1 << " dEdxrest " << dEdx2 << " cutRange " << cutInRange << " <-> " << cut << G4endl; //GDEB
  
  return dEdx;
}

//----------------------------------------------------------------
G4double GmDataInitialStoppingPower::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  const G4LogicalVolume* LV = aTrack->GetLogicalVolumeAtVertex();
  G4double dEdx = theEMCalculator.GetDEDX(aTrack->GetVertexKineticEnergy(),
					  aTrack->GetParticleDefinition(),
					  LV->GetMaterial(),
					  LV->GetRegion());
  return dEdx;
}

//----------------------------------------------------------------
G4double GmDataInitialStoppingPower::GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track* , G4int )
{
  const G4LogicalVolume* LV = aTrack1->GetLogicalVolumeAtVertex();
  G4double dEdx = theEMCalculator.GetDEDX(aTrack1->GetVertexKineticEnergy(),
					  aTrack1->GetParticleDefinition(),
					  LV->GetMaterial(),
					  LV->GetRegion());
  return dEdx;
}


 //----------------------------------------------------------------
G4double GmDataInitialStoppingPower::GetValueFromStackedTrack( const G4Track* aTrack, G4int )
{
  G4VPhysicalVolume* PV = GetPVFromPos(aTrack->GetPosition());
  G4LogicalVolume* LV = PV->GetLogicalVolume();
  G4double dEdx = theEMCalculator.GetDEDX(aTrack->GetVertexKineticEnergy(),
					  aTrack->GetParticleDefinition(),
					  LV->GetMaterial(),
					  LV->GetRegion());
  return dEdx;

}

