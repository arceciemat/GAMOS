#include "GmDataSecoKineticEnergy.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataSecoKineticEnergy::GmDataSecoKineticEnergy()
{
  theHMin = 0.;
  theHMax = 1.;
  theExcludedTypes.insert(DTStep);
  theExcludedTypes.insert(DTTrack);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataSecoKineticEnergy::~GmDataSecoKineticEnergy()
{
}


//----------------------------------------------------------------
G4double GmDataSecoKineticEnergy::GetValueFromSecoTrack( const G4Track* , const G4Track* aTrack2, G4int )
{
  return aTrack2->GetKineticEnergy();
}
