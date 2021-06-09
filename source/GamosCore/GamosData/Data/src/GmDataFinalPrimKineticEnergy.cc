#include "GmDataFinalPrimKineticEnergy.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataFinalPrimKineticEnergy::GmDataFinalPrimKineticEnergy()
{
  bInitial = false;
  theHMin = 0.;
  theHMax = 1.;
  theExcludedTypes.insert(DTStep);
  theExcludedTypes.insert(DTTrack);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalPrimKineticEnergy::~GmDataFinalPrimKineticEnergy()
{
}


//----------------------------------------------------------------
G4double GmDataFinalPrimKineticEnergy::GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track*, G4int )
{
  return aTrack1->GetKineticEnergy();
}
