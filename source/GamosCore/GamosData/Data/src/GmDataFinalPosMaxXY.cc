#include "GmDataFinalPosMaxXY.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataFinalPosMaxXY::GmDataFinalPosMaxXY()
{
  bInitial = true;
  theHMin = -1000.;
  theHMax = 1000.;
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalPosMaxXY::~GmDataFinalPosMaxXY()
{
}


//----------------------------------------------------------------
G4double GmDataFinalPosMaxXY::GetValueFromStep( const G4Step* aStep, G4int )
{
  return std::max(fabs(aStep->GetPostStepPoint()->GetPosition().x()),fabs(aStep->GetPostStepPoint()->GetPosition().y()));
}

//----------------------------------------------------------------
G4double GmDataFinalPosMaxXY::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return std::max(fabs(aTrack->GetPosition().x()),fabs(aTrack->GetPosition().y()));
}

//----------------------------------------------------------------
G4double GmDataFinalPosMaxXY::GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track*, G4int )
{
  return std::max(fabs(aTrack1->GetPosition().x()),fabs(aTrack1->GetPosition().y()));
}


