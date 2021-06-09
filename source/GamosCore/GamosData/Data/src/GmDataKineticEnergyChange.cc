#include "GmDataKineticEnergyChange.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataKineticEnergyChange::GmDataKineticEnergyChange()
{
  theHMin = 0.;
  theHMax = 1.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataKineticEnergyChange::~GmDataKineticEnergyChange()
{
}


//----------------------------------------------------------------
G4double GmDataKineticEnergyChange::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPreStepPoint()->GetKineticEnergy() - aStep->GetPostStepPoint()->GetKineticEnergy();
}

//----------------------------------------------------------------
G4double GmDataKineticEnergyChange::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetVertexKineticEnergy() - aTrack->GetKineticEnergy();
}
