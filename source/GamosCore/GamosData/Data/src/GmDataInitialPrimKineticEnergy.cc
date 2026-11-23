#include "GmDataInitialPrimKineticEnergy.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataInitialPrimKineticEnergy::GmDataInitialPrimKineticEnergy()
{
  bInitial = true;
  theHMin = 0.;
  theHMax = 1.;
  theExcludedTypes.insert(DTStep);
  theExcludedTypes.insert(DTTrack);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataInitialPrimKineticEnergy::~GmDataInitialPrimKineticEnergy()
{
}


//----------------------------------------------------------------
G4double GmDataInitialPrimKineticEnergy::GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track*, G4int )
{
  return aTrack1->GetStep()->GetPreStepPoint()->GetKineticEnergy();
}
