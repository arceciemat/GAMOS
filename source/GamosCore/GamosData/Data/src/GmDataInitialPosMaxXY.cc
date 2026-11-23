#include "GmDataInitialPosMaxXY.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmDataInitialPosMaxXY::GmDataInitialPosMaxXY()
{
  bInitial = true;
  theHMin = -1000.;
  theHMax = 1000.;
}

//----------------------------------------------------------------
GmDataInitialPosMaxXY::~GmDataInitialPosMaxXY()
{
}


//----------------------------------------------------------------
G4double GmDataInitialPosMaxXY::GetValueFromStep( const G4Step* aStep, G4int )
{

  return std::max(fabs(aStep->GetPreStepPoint()->GetPosition().x()),fabs(aStep->GetPreStepPoint()->GetPosition().y()));
}

//----------------------------------------------------------------
G4double GmDataInitialPosMaxXY::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return std::max(fabs(aTrack->GetVertexPosition().x()),fabs(aTrack->GetVertexPosition().y()));
}

//----------------------------------------------------------------
G4double GmDataInitialPosMaxXY::GetValueFromSecoTrack( const G4Track* , const G4Track* aTrack2, G4int )
{
  return std::max(fabs(aTrack2->GetPosition().x()),fabs(aTrack2->GetPosition().y()));
}

//----------------------------------------------------------------
G4double GmDataInitialPosMaxXY::GetValueFromEvent( const G4Event* anEvent, G4int )
{
  return std::max(fabs(anEvent->GetPrimaryVertex(0)->GetX0()),fabs(anEvent->GetPrimaryVertex(0)->GetY0()));
}

//----------------------------------------------------------------
G4double GmDataInitialPosMaxXY::GetValueFromStackedTrack( const G4Track* aTrack, G4int )
{
  return std::max(fabs(aTrack->GetPosition().x()),fabs(aTrack->GetPosition().y()));
}

