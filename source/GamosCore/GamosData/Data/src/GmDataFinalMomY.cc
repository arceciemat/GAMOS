#include "GmDataFinalMomY.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataFinalMomY::GmDataFinalMomY()
{
  bInitial = false;
  theHMin = -1.;
  theHMax = 1.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalMomY::~GmDataFinalMomY()
{
}


//----------------------------------------------------------------
G4double GmDataFinalMomY::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPostStepPoint()->GetMomentum().y();
}

//----------------------------------------------------------------
G4double GmDataFinalMomY::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetMomentum().y();
}
