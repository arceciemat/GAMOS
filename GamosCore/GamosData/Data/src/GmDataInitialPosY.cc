#include "GmDataInitialPosY.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmDataInitialPosY::GmDataInitialPosY()
{
  bInitial = true;
  theHMin = -1000.;
  theHMax = 1000.;
}

//----------------------------------------------------------------
GmDataInitialPosY::~GmDataInitialPosY()
{
}


//----------------------------------------------------------------
G4double GmDataInitialPosY::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPreStepPoint()->GetPosition().y();
}

//----------------------------------------------------------------
G4double GmDataInitialPosY::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetVertexPosition().y();
}

//----------------------------------------------------------------
G4double GmDataInitialPosY::GetValueFromSecoTrack( const G4Track* , const G4Track* aTrack2, G4int )
{
  return aTrack2->GetPosition().y();
}


//----------------------------------------------------------------
G4double GmDataInitialPosY::GetValueFromEvent( const G4Event* aEvent, G4int )
{
  return aEvent->GetPrimaryVertex(0)->GetY0();
}
