#include "GmDataInitialPosX.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmDataInitialPosX::GmDataInitialPosX()
{
  bInitial = true;
  theHMin = -1000.;
  theHMax = 1000.;
}

//----------------------------------------------------------------
GmDataInitialPosX::~GmDataInitialPosX()
{
}


//----------------------------------------------------------------
G4double GmDataInitialPosX::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPreStepPoint()->GetPosition().x();
}

//----------------------------------------------------------------
G4double GmDataInitialPosX::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetVertexPosition().x();
}

//----------------------------------------------------------------
G4double GmDataInitialPosX::GetValueFromSecoTrack( const G4Track* , const G4Track* aTrack2, G4int )
{
  return aTrack2->GetPosition().x();
}

//----------------------------------------------------------------
G4double GmDataInitialPosX::GetValueFromEvent( const G4Event* anEvent, G4int )
{
  return anEvent->GetPrimaryVertex(0)->GetX0();
}

//----------------------------------------------------------------
G4double GmDataInitialPosX::GetValueFromStackedTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetPosition().x();
}
