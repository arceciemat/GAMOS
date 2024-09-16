#include "GmDataInitialElectronicStoppingPower.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4VPhysicalVolume.hh"
#include "G4TouchableHistory.hh"
#include "G4TransportationManager.hh"
#include "G4PVParameterised.hh"

//----------------------------------------------------------------
GmDataInitialElectronicStoppingPower::GmDataInitialElectronicStoppingPower()
{
  bInitial = true;
  theHMin = 0.;
  theHMax = 10.;

}

//----------------------------------------------------------------
GmDataInitialElectronicStoppingPower::~GmDataInitialElectronicStoppingPower()
{
}


//----------------------------------------------------------------
G4double GmDataInitialElectronicStoppingPower::GetValueFromStep( const G4Step* aStep, G4int )
{
  G4StepPoint* preStep = aStep->GetPreStepPoint();
  G4double dEdx = theEMCalculator.ComputeElectronicDEDX(preStep->GetKineticEnergy(),
						   aStep->GetTrack()->GetParticleDefinition(),
						   preStep->GetMaterial(),
						   DBL_MAX);
   
  return dEdx;
}

//----------------------------------------------------------------
G4double GmDataInitialElectronicStoppingPower::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  const G4LogicalVolume* LV = aTrack->GetLogicalVolumeAtVertex();
  G4double dEdx = theEMCalculator.ComputeElectronicDEDX(aTrack->GetVertexKineticEnergy(),
							aTrack->GetParticleDefinition(),
							LV->GetMaterial(),
							DBL_MAX);
  return dEdx;
}

//----------------------------------------------------------------
G4double GmDataInitialElectronicStoppingPower::GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track* , G4int )
{
  const G4LogicalVolume* LV = aTrack1->GetLogicalVolumeAtVertex();
  G4double dEdx = theEMCalculator.ComputeElectronicDEDX(aTrack1->GetVertexKineticEnergy(),
							aTrack1->GetParticleDefinition(),
							LV->GetMaterial(),
							DBL_MAX);
  return dEdx;
}


 //----------------------------------------------------------------
G4double GmDataInitialElectronicStoppingPower::GetValueFromStackedTrack( const G4Track* aTrack, G4int )
{
  G4VPhysicalVolume* PV = GetPVFromPos(aTrack->GetPosition());
  G4LogicalVolume* LV = PV->GetLogicalVolume();
  G4double dEdx = theEMCalculator.ComputeElectronicDEDX(aTrack->GetVertexKineticEnergy(),
							aTrack->GetParticleDefinition(),
							LV->GetMaterial(),
						        DBL_MAX);
  return dEdx;

}

