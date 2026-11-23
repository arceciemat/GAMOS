#include "GmDataFinalMomTheta.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataFinalMomTheta::GmDataFinalMomTheta()
{
  bInitial = false;
  theHMin = 0.;
  theHMax = 180.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalMomTheta::~GmDataFinalMomTheta()
{
}


//----------------------------------------------------------------
G4double GmDataFinalMomTheta::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPostStepPoint()->GetMomentum().theta()/CLHEP::deg;
}

//----------------------------------------------------------------
G4double GmDataFinalMomTheta::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetMomentum().theta()/CLHEP::deg;
}
