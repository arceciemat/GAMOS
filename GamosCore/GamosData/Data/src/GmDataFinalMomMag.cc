#include "GmDataFinalMomMag.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataFinalMomMag::GmDataFinalMomMag()
{
  bInitial = false;
  theHMin = 0.;
  theHMax = 1.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalMomMag::~GmDataFinalMomMag()
{
}


//----------------------------------------------------------------
G4double GmDataFinalMomMag::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPostStepPoint()->GetMomentum().mag();
}

//----------------------------------------------------------------
G4double GmDataFinalMomMag::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetMomentum().mag();
}
