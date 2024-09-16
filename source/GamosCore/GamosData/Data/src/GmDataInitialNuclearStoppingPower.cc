#include "GmDataInitialNuclearStoppingPower.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4VPhysicalVolume.hh"
#include "G4TouchableHistory.hh"
#include "G4TransportationManager.hh"
#include "G4PVParameterised.hh"

//----------------------------------------------------------------
GmDataInitialNuclearStoppingPower::GmDataInitialNuclearStoppingPower()
{
  bInitial = true;
  theHMin = 0.;
  theHMax = 10.;

}

//----------------------------------------------------------------
GmDataInitialNuclearStoppingPower::~GmDataInitialNuclearStoppingPower()
{
}
// ONLY != 0 IF  if(kinEnergy*proton_mass_c2/mass1 > z1*z1*MeV); mass1=particle_mass , z1=p->GetPDGCharge()/eplus  ( G4ICRU49NuclearStoppingModel::ComputeDEDXPerVolume( )

//----------------------------------------------------------------
G4double GmDataInitialNuclearStoppingPower::GetValueFromStep( const G4Step* aStep, G4int )
{
  G4StepPoint* preStep = aStep->GetPreStepPoint();
  G4double dEdx = theEMCalculator.ComputeNuclearDEDX(preStep->GetKineticEnergy(),
						   aStep->GetTrack()->GetParticleDefinition(),
						     preStep->GetMaterial());
   
  return dEdx;
}

//----------------------------------------------------------------
G4double GmDataInitialNuclearStoppingPower::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  const G4LogicalVolume* LV = aTrack->GetLogicalVolumeAtVertex();
  G4double dEdx = theEMCalculator.ComputeNuclearDEDX(aTrack->GetVertexKineticEnergy(),
							aTrack->GetParticleDefinition(),
						     LV->GetMaterial());
  return dEdx;
}

//----------------------------------------------------------------
G4double GmDataInitialNuclearStoppingPower::GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track* , G4int )
{
  const G4LogicalVolume* LV = aTrack1->GetLogicalVolumeAtVertex();
  G4double dEdx = theEMCalculator.ComputeNuclearDEDX(aTrack1->GetVertexKineticEnergy(),
							aTrack1->GetParticleDefinition(),
						     LV->GetMaterial());
  return dEdx;
}


 //----------------------------------------------------------------
G4double GmDataInitialNuclearStoppingPower::GetValueFromStackedTrack( const G4Track* aTrack, G4int )
{
  G4VPhysicalVolume* PV = GetPVFromPos(aTrack->GetPosition());
  G4LogicalVolume* LV = PV->GetLogicalVolume();
  G4double dEdx = theEMCalculator.ComputeNuclearDEDX(aTrack->GetVertexKineticEnergy(),
							aTrack->GetParticleDefinition(),
						     LV->GetMaterial());
  return dEdx;

}

