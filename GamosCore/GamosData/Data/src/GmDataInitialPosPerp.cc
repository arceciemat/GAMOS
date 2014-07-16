#include "GmDataInitialPosPerp.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmDataInitialPosPerp::GmDataInitialPosPerp()
{
  bInitial = true;
  theHMin = 0.;
  theHMax = 1000.;
}

//----------------------------------------------------------------
GmDataInitialPosPerp::~GmDataInitialPosPerp()
{
}


//----------------------------------------------------------------
G4double GmDataInitialPosPerp::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPreStepPoint()->GetPosition().perp();
}

//----------------------------------------------------------------
G4double GmDataInitialPosPerp::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetVertexPosition().perp();
}

//----------------------------------------------------------------
G4double GmDataInitialPosPerp::GetValueFromSecoTrack( const G4Track* , const G4Track* aTrack2, G4int )
{
  return aTrack2->GetPosition().perp();
}


//----------------------------------------------------------------
G4double GmDataInitialPosPerp::GetValueFromEvent( const G4Event* aEvent, G4int )
{
  return aEvent->GetPrimaryVertex(0)->GetPosition().perp();
}
