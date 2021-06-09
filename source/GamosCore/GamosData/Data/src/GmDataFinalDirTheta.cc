#include "GmDataFinalDirTheta.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataFinalDirTheta::GmDataFinalDirTheta()
{
  bInitial = false;
  theHMin = 0.;
  theHMax = 180.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalDirTheta::~GmDataFinalDirTheta()
{
}


//----------------------------------------------------------------
G4double GmDataFinalDirTheta::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPostStepPoint()->GetMomentumDirection().theta()/CLHEP::deg;
}

//----------------------------------------------------------------
G4double GmDataFinalDirTheta::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetMomentumDirection().theta()/CLHEP::deg;
}
