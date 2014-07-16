#include "GmDataFinalMomPhi.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataFinalMomPhi::GmDataFinalMomPhi()
{
  bInitial = false;
  theHMin = -180.;
  theHMax = 180.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalMomPhi::~GmDataFinalMomPhi()
{
}


//----------------------------------------------------------------
G4double GmDataFinalMomPhi::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPostStepPoint()->GetMomentum().phi()/CLHEP::deg;
}

//----------------------------------------------------------------
G4double GmDataFinalMomPhi::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetMomentum().phi()/CLHEP::deg;
}
