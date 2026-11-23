#include "GmDataInitialPrimMinusSecoKineticEnergy.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataInitialPrimMinusSecoKineticEnergy::GmDataInitialPrimMinusSecoKineticEnergy()
{
  bInitial = true;
  theHMin = 0.;
  theHMax = 1.;
  theExcludedTypes.insert(DTStep);
  theExcludedTypes.insert(DTTrack);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataInitialPrimMinusSecoKineticEnergy::~GmDataInitialPrimMinusSecoKineticEnergy()
{
}


//----------------------------------------------------------------
G4double GmDataInitialPrimMinusSecoKineticEnergy::GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track* aTrack2, G4int )
{
  return aTrack1->GetStep()->GetPreStepPoint()->GetKineticEnergy() - aTrack2->GetKineticEnergy();
}
