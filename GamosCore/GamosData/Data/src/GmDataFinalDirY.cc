#include "GmDataFinalDirY.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataFinalDirY::GmDataFinalDirY()
{
  bInitial = false;
  theHMin = -1.;
  theHMax = 1.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalDirY::~GmDataFinalDirY()
{
}


//----------------------------------------------------------------
G4double GmDataFinalDirY::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPostStepPoint()->GetMomentumDirection().y();
}

//----------------------------------------------------------------
G4double GmDataFinalDirY::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetMomentumDirection().y();
}
