#include "GmDataVertexPrimKineticEnergy.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataVertexPrimKineticEnergy::GmDataVertexPrimKineticEnergy()
{
  bInitial = true;
  theHMin = 0.;
  theHMax = 1.;
  theExcludedTypes.insert(DTStep);
  theExcludedTypes.insert(DTTrack);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataVertexPrimKineticEnergy::~GmDataVertexPrimKineticEnergy()
{
}


//----------------------------------------------------------------
G4double GmDataVertexPrimKineticEnergy::GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track*, G4int )
{
  return aTrack1->GetVertexKineticEnergy();
}
