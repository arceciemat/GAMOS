#include "GmDataInitialTime.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmDataInitialTime::GmDataInitialTime()
{
  bInitial = true;
  theHMin = 0.;
  theHMax = 1000.;
}

//----------------------------------------------------------------
GmDataInitialTime::~GmDataInitialTime()
{
}


//----------------------------------------------------------------
G4double GmDataInitialTime::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPreStepPoint()->GetGlobalTime();
}

//----------------------------------------------------------------
G4double GmDataInitialTime::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetGlobalTime();
}

//----------------------------------------------------------------
G4double GmDataInitialTime::GetValueFromSecoTrack( const G4Track* aTrack1 , const G4Track*, G4int )
{
  return aTrack1->GetGlobalTime();
}

//----------------------------------------------------------------
G4double GmDataInitialTime::GetValueFromEvent( const G4Event* anEvent, G4int )
{

  return anEvent->GetPrimaryVertex(0)->GetT0();
}
