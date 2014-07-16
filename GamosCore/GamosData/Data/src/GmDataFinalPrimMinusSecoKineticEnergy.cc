#include "GmDataFinalPrimMinusSecoKineticEnergy.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataFinalPrimMinusSecoKineticEnergy::GmDataFinalPrimMinusSecoKineticEnergy()
{
  bInitial = false;
  theHMin = 0.;
  theHMax = 1.;
  theExcludedTypes.insert(DTStep);
  theExcludedTypes.insert(DTTrack);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalPrimMinusSecoKineticEnergy::~GmDataFinalPrimMinusSecoKineticEnergy()
{
}


//----------------------------------------------------------------
G4double GmDataFinalPrimMinusSecoKineticEnergy::GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track* aTrack2, G4int )
{
  return aTrack1->GetKineticEnergy() - aTrack2->GetKineticEnergy();
}
