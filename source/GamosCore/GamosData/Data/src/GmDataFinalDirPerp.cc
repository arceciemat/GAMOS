#include "GmDataFinalDirPerp.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataFinalDirPerp::GmDataFinalDirPerp()
{
  bInitial = false;
  theHMin = 0.;
  theHMax = 1.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalDirPerp::~GmDataFinalDirPerp()
{
}


//----------------------------------------------------------------
G4double GmDataFinalDirPerp::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPostStepPoint()->GetMomentumDirection().perp();
}

//----------------------------------------------------------------
G4double GmDataFinalDirPerp::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetMomentumDirection().perp();
}
