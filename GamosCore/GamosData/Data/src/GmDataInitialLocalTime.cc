#include "GmDataInitialLocalTime.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmDataInitialLocalTime::GmDataInitialLocalTime()
{
  bInitial = true;
  theHMin = 0.;
  theHMax = 1000.;
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataInitialLocalTime::~GmDataInitialLocalTime()
{
}


//----------------------------------------------------------------
G4double GmDataInitialLocalTime::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPreStepPoint()->GetLocalTime();
}

//----------------------------------------------------------------
G4double GmDataInitialLocalTime::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetLocalTime();
}

//----------------------------------------------------------------
G4double GmDataInitialLocalTime::GetValueFromSecoTrack( const G4Track* , const G4Track*, G4int )
{

  return 0.;
}
