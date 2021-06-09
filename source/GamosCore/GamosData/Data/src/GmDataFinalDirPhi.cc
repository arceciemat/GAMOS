#include "GmDataFinalDirPhi.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataFinalDirPhi::GmDataFinalDirPhi()
{
  bInitial = false;
  theHMin = -180.;
  theHMax = 180.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalDirPhi::~GmDataFinalDirPhi()
{
}


//----------------------------------------------------------------
G4double GmDataFinalDirPhi::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPostStepPoint()->GetMomentumDirection().phi()/CLHEP::deg;
}

//----------------------------------------------------------------
G4double GmDataFinalDirPhi::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetMomentumDirection().phi()/CLHEP::deg;
}
