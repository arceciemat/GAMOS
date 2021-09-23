#include "GmDataSecoDirTheta.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataSecoDirTheta::GmDataSecoDirTheta()
{
  theHMin = 0.;
  theHMax = 180.;
  theExcludedTypes.insert(DTStep);
  theExcludedTypes.insert(DTTrack);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataSecoDirTheta::~GmDataSecoDirTheta()
{
}


//----------------------------------------------------------------
G4double GmDataSecoDirTheta::GetValueFromSecoTrack( const G4Track* , const G4Track* aTrack2, G4int )
{ 
  return aTrack2->GetMomentumDirection().theta()/CLHEP::deg;
}
