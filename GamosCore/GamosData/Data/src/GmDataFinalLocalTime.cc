#include "GmDataFinalLocalTime.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataFinalLocalTime::GmDataFinalLocalTime()
{
  bInitial = false;
  theHMin = 0.;
  theHMax = 1000.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalLocalTime::~GmDataFinalLocalTime()
{
}


//----------------------------------------------------------------
G4double GmDataFinalLocalTime::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPostStepPoint()->GetLocalTime();
}

//----------------------------------------------------------------
G4double GmDataFinalLocalTime::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetLocalTime();
}
