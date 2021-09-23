#include "GmDataFinalPosPerp.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataFinalPosPerp::GmDataFinalPosPerp()
{
  bInitial = false;
  theHMin = 0.;
  theHMax = 1000.;
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalPosPerp::~GmDataFinalPosPerp()
{
}


//----------------------------------------------------------------
G4double GmDataFinalPosPerp::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPostStepPoint()->GetPosition().perp();
}

//----------------------------------------------------------------
G4double GmDataFinalPosPerp::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetPosition().perp();
}

//----------------------------------------------------------------
G4double GmDataFinalPosPerp::GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track*, G4int )
{
  return aTrack1->GetPosition().perp();
}
