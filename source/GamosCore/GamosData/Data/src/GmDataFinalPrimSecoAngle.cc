#include "GmDataFinalPrimSecoAngle.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataFinalPrimSecoAngle::GmDataFinalPrimSecoAngle()
{
  bInitial = false;
  theHMin = 0.;
  theHMax = 180.;
  theExcludedTypes.insert(DTStep);
  theExcludedTypes.insert(DTTrack);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalPrimSecoAngle::~GmDataFinalPrimSecoAngle()
{
}


//----------------------------------------------------------------
G4double GmDataFinalPrimSecoAngle::GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track* aTrack2, G4int )
{
  return acos(aTrack1->GetStep()->GetPostStepPoint()->GetMomentumDirection() * aTrack2->GetMomentumDirection() )/CLHEP::deg;
}
