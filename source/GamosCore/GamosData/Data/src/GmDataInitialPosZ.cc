#include "GmDataInitialPosZ.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmDataInitialPosZ::GmDataInitialPosZ()
{
  bInitial = true;
  theHMin = -1000.;
  theHMax = 1000.;
}

//----------------------------------------------------------------
GmDataInitialPosZ::~GmDataInitialPosZ()
{
}


//----------------------------------------------------------------
G4double GmDataInitialPosZ::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPreStepPoint()->GetPosition().z();
}

//----------------------------------------------------------------
G4double GmDataInitialPosZ::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetVertexPosition().z();
}

//----------------------------------------------------------------
G4double GmDataInitialPosZ::GetValueFromSecoTrack( const G4Track* , const G4Track* aTrack2, G4int )
{
  return aTrack2->GetPosition().z();
}


//----------------------------------------------------------------
G4double GmDataInitialPosZ::GetValueFromEvent( const G4Event* aEvent, G4int )
{
  return aEvent->GetPrimaryVertex(0)->GetZ0();
}

//----------------------------------------------------------------
G4double GmDataInitialPosZ::GetValueFromStackedTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetPosition().z();
}
