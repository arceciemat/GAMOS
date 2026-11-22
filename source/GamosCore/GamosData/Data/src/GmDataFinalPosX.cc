#include "GmDataFinalPosX.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataFinalPosX::GmDataFinalPosX()
{
  bInitial = false;
  theHMin = -1000.;
  theHMax = 1000.;
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalPosX::~GmDataFinalPosX()
{
}


//----------------------------------------------------------------
G4double GmDataFinalPosX::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPostStepPoint()->GetPosition().x();
}

//----------------------------------------------------------------
G4double GmDataFinalPosX::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetPosition().x();
}

//----------------------------------------------------------------
G4double GmDataFinalPosX::GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track*, G4int )
{
  return aTrack1->GetPosition().x();
}
