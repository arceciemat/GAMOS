#include "GmDataSecoInitialDirTheta.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataSecoInitialDirTheta::GmDataSecoInitialDirTheta()
{
  theHMin = 0.;
  theHMax = 180.;
  theExcludedTypes.insert(DTStep);
  theExcludedTypes.insert(DTTrack);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataSecoInitialDirTheta::~GmDataSecoInitialDirTheta()
{
}


//----------------------------------------------------------------
G4double GmDataSecoInitialDirTheta::GetValueFromSecoTrack( const G4Track* , const G4Track* aTrack2, G4int )
{
  return aTrack2->GetMomentumDirection().theta()/CLHEP::deg;

}
