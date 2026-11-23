#include "GmDataFinalProperTime.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataFinalProperTime::GmDataFinalProperTime()
{
  bInitial = false;
  theHMin = 0.;
  theHMax = 1000.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalProperTime::~GmDataFinalProperTime()
{
}


//----------------------------------------------------------------
G4double GmDataFinalProperTime::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPostStepPoint()->GetProperTime();
}

//----------------------------------------------------------------
G4double GmDataFinalProperTime::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetProperTime();
}
