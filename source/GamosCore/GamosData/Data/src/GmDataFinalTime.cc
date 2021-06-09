#include "GmDataFinalTime.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataFinalTime::GmDataFinalTime()
{
  bInitial = false;
  theHMin = 0.;
  theHMax = 1000.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalTime::~GmDataFinalTime()
{
}


//----------------------------------------------------------------
G4double GmDataFinalTime::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPostStepPoint()->GetGlobalTime();
}

//----------------------------------------------------------------
G4double GmDataFinalTime::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetGlobalTime();
}
