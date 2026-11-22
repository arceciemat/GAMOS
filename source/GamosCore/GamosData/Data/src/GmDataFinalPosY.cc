#include "GmDataFinalPosY.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataFinalPosY::GmDataFinalPosY()
{
  bInitial = false;
  theHMin = -1000.;
  theHMax = 1000.;
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalPosY::~GmDataFinalPosY()
{
}


//----------------------------------------------------------------
G4double GmDataFinalPosY::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPostStepPoint()->GetPosition().y();
}

//----------------------------------------------------------------
G4double GmDataFinalPosY::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetPosition().y();
}

//----------------------------------------------------------------
G4double GmDataFinalPosY::GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track*, G4int )
{
  return aTrack1->GetPosition().y();
}
