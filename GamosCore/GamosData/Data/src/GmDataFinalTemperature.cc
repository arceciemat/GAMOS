#include "GmDataFinalTemperature.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmDataFinalTemperature::GmDataFinalTemperature()
{
  bInitial = false;
  theHMin = 0.;
  theHMax = 1000.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalTemperature::~GmDataFinalTemperature()
{
}


//----------------------------------------------------------------
G4double GmDataFinalTemperature::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPostStepPoint()->GetMaterial()->GetTemperature();
}

//----------------------------------------------------------------
G4double GmDataFinalTemperature::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetNextMaterial()->GetTemperature();
}
