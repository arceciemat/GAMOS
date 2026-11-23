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
  G4Material* mate = aStep->GetPostStepPoint()->GetMaterial();
  if( mate ) {
    return mate->GetTemperature();
  } else {
    return 0.;
  }
}

//----------------------------------------------------------------
G4double GmDataFinalTemperature::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  G4Material* mate = aTrack->GetNextMaterial();
  if( mate ) {
    return mate->GetTemperature();
  } else {
    return 0.;
  }
}
