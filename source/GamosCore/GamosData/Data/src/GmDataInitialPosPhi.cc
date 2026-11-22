#include "GmDataInitialPosPhi.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmDataInitialPosPhi::GmDataInitialPosPhi()
{
  bInitial = true;
  theHMin = -180.;
  theHMax = 180.;
}

//----------------------------------------------------------------
GmDataInitialPosPhi::~GmDataInitialPosPhi()
{
}


//----------------------------------------------------------------
G4double GmDataInitialPosPhi::GetValueFromStep( const G4Step* aStep, G4int )
{
  //  G4cout << " GmDataInitalPosPhi::GetValueFromStep " << aStep->GetPreStepPoint()->GetPosition().phi()/CLHEP::deg << " " << aStep->GetPreStepPoint()->GetPosition() << G4endl;
  return aStep->GetPreStepPoint()->GetPosition().phi()/CLHEP::deg;
}

//----------------------------------------------------------------
G4double GmDataInitialPosPhi::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetVertexPosition().phi()/CLHEP::deg;
}

//----------------------------------------------------------------
G4double GmDataInitialPosPhi::GetValueFromSecoTrack( const G4Track* , const G4Track* aTrack2, G4int )
{
  return aTrack2->GetPosition().phi()/CLHEP::deg;
}


//----------------------------------------------------------------
G4double GmDataInitialPosPhi::GetValueFromEvent( const G4Event* aEvent, G4int )
{
  return aEvent->GetPrimaryVertex(0)->GetPosition().phi();
}

//----------------------------------------------------------------
G4double GmDataInitialPosPhi::GetValueFromStackedTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetPosition().phi();
}
