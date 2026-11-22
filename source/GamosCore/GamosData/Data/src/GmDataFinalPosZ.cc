#include "GmDataFinalPosZ.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataFinalPosZ::GmDataFinalPosZ()
{
  bInitial = false;
  theHMin = -1000.;
  theHMax = 1000.;
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalPosZ::~GmDataFinalPosZ()
{
}


//----------------------------------------------------------------
G4double GmDataFinalPosZ::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPostStepPoint()->GetPosition().z();
}

//----------------------------------------------------------------
G4double GmDataFinalPosZ::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetPosition().z();
}

//----------------------------------------------------------------
G4double GmDataFinalPosZ::GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track*, G4int )
{
  return aTrack1->GetPosition().z();
}


