#include "GmDataStepNumber.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmDataStepNumber::GmDataStepNumber()
{
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataStepNumber::~GmDataStepNumber()
{
}


//----------------------------------------------------------------
G4double GmDataStepNumber::GetValueFromStep( const G4Step* aStep, G4int )
{

  return aStep->GetTrack()->GetCurrentStepNumber();

}

//----------------------------------------------------------------
G4double GmDataStepNumber::GetValueFromTrack( const G4Track* aTrack, G4int )
{

  return aTrack->GetCurrentStepNumber();
}
