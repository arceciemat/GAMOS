#include "GmDataSecoDividedFinalPrimEnergy.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataSecoDividedFinalPrimEnergy::GmDataSecoDividedFinalPrimEnergy()
{
  theHMin = 0.;
  theHMax = 1.;
  theExcludedTypes.insert(DTStep);
  theExcludedTypes.insert(DTTrack);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataSecoDividedFinalPrimEnergy::~GmDataSecoDividedFinalPrimEnergy()
{
}


//----------------------------------------------------------------
G4double GmDataSecoDividedFinalPrimEnergy::GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track* aTrack2, G4int )
{
  return  aTrack2->GetKineticEnergy() / aTrack1->GetKineticEnergy();
}
