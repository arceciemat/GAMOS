#include "GmDataInitialPosTheta.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmDataInitialPosTheta::GmDataInitialPosTheta()
{
  bInitial = true;
  theHMin = 0.;
  theHMax = 180.;
}

//----------------------------------------------------------------
GmDataInitialPosTheta::~GmDataInitialPosTheta()
{
}


//----------------------------------------------------------------
G4double GmDataInitialPosTheta::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPreStepPoint()->GetPosition().theta()/CLHEP::deg;
}

//----------------------------------------------------------------
G4double GmDataInitialPosTheta::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetVertexPosition().theta()/CLHEP::deg;
}

//----------------------------------------------------------------
G4double GmDataInitialPosTheta::GetValueFromSecoTrack( const G4Track* , const G4Track* aTrack2, G4int )
{
  return aTrack2->GetPosition().theta()/CLHEP::deg;
}



//----------------------------------------------------------------
G4double GmDataInitialPosTheta::GetValueFromEvent( const G4Event* aEvent, G4int )
{
  return aEvent->GetPrimaryVertex(0)->GetPosition().theta();
}
