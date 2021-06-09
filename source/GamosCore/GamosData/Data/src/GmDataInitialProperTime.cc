#include "GmDataInitialProperTime.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmDataInitialProperTime::GmDataInitialProperTime()
{
  bInitial = true;
  theHMin = 0.;
  theHMax = 1000.;
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataInitialProperTime::~GmDataInitialProperTime()
{
}


//----------------------------------------------------------------
G4double GmDataInitialProperTime::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPreStepPoint()->GetProperTime();
}

//----------------------------------------------------------------
G4double GmDataInitialProperTime::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetProperTime();
}

//----------------------------------------------------------------
G4double GmDataInitialProperTime::GetValueFromSecoTrack( const G4Track* , const G4Track*, G4int )
{

  return 0.;
}
