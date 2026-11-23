#include "GmDataPrimParticle.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataPrimParticle::GmDataPrimParticle()
{
  theNChar = 15;
  theNBytes = theNChar;

  theExcludedTypes.insert(DTStep);
  theExcludedTypes.insert(DTTrack);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataPrimParticle::~GmDataPrimParticle()
{
}


//----------------------------------------------------------------
G4String GmDataPrimParticle::GetStringValueFromSecoTrack( const G4Track* aTrack1, const G4Track* )
{
  return aTrack1->GetDefinition()->GetParticleName();
}
