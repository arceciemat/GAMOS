#include "GmDataFinalDirZ.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataFinalDirZ::GmDataFinalDirZ()
{
  bInitial = false;
  theHMin = -1.;
  theHMax = 1.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalDirZ::~GmDataFinalDirZ()
{
}


//----------------------------------------------------------------
G4double GmDataFinalDirZ::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPostStepPoint()->GetMomentumDirection().z();
}

//----------------------------------------------------------------
G4double GmDataFinalDirZ::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetMomentumDirection().z();
}
