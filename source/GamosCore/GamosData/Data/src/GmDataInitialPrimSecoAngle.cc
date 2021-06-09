#include "GmDataInitialPrimSecoAngle.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataInitialPrimSecoAngle::GmDataInitialPrimSecoAngle()
{
  bInitial = true;
  theHMin = 0.;
  theHMax = 180.;
  theExcludedTypes.insert(DTStep);
  theExcludedTypes.insert(DTTrack);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataInitialPrimSecoAngle::~GmDataInitialPrimSecoAngle()
{
}


//----------------------------------------------------------------
G4double GmDataInitialPrimSecoAngle::GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track* aTrack2, G4int )
{
  return acos(aTrack1->GetStep()->GetPreStepPoint()->GetMomentumDirection() * aTrack2->GetMomentumDirection() )/CLHEP::deg;
}
