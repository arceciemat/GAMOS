#include "GmDataFinalDirX.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataFinalDirX::GmDataFinalDirX()
{
  bInitial = false;
  theHMin = -1.;
  theHMax = 1.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalDirX::~GmDataFinalDirX()
{
}


//----------------------------------------------------------------
G4double GmDataFinalDirX::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPostStepPoint()->GetMomentumDirection().x();
}

//----------------------------------------------------------------
G4double GmDataFinalDirX::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetMomentumDirection().x();
}
