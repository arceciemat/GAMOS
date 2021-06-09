#include "GmDataFinalMomPerp.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataFinalMomPerp::GmDataFinalMomPerp()
{
  bInitial = false;
  theHMin = 0.;
  theHMax = 1.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalMomPerp::~GmDataFinalMomPerp()
{
}


//----------------------------------------------------------------
G4double GmDataFinalMomPerp::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPostStepPoint()->GetMomentum().perp();
}

//----------------------------------------------------------------
G4double GmDataFinalMomPerp::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetMomentum().perp();
}
