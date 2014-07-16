#include "GmDataInitialPosMag.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmDataInitialPosMag::GmDataInitialPosMag()
{
  bInitial = true;
  theHMin = 0.;
  theHMax = 1000.;
}

//----------------------------------------------------------------
GmDataInitialPosMag::~GmDataInitialPosMag()
{
}


//----------------------------------------------------------------
G4double GmDataInitialPosMag::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPreStepPoint()->GetPosition().mag();
}

//----------------------------------------------------------------
G4double GmDataInitialPosMag::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetVertexPosition().mag();
}

//----------------------------------------------------------------
G4double GmDataInitialPosMag::GetValueFromSecoTrack( const G4Track* , const G4Track* aTrack2, G4int )
{
  return aTrack2->GetPosition().mag();
}

//----------------------------------------------------------------
G4double GmDataInitialPosMag::GetValueFromEvent( const G4Event* aEvent, G4int )
{
  return aEvent->GetPrimaryVertex(0)->GetPosition().mag();
}
