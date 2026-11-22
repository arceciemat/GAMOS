#include "GmDataPrimaryParticle.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataPrimaryParticle::GmDataPrimaryParticle()
{
  theNChar = 15;
  theNBytes = theNChar;

  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataPrimaryParticle::~GmDataPrimaryParticle()
{
}

 //----------------------------------------------------------------
G4String GmDataPrimaryParticle::GetStringValueFromStep( const G4Step* aStep )
{ 
  return aStep->GetTrack()->GetDefinition()->GetParticleName();
}

//----------------------------------------------------------------
G4String GmDataPrimaryParticle::GetStringValueFromTrack( const G4Track* aTrack )
{ 
  return aTrack->GetDefinition()->GetParticleName();
}

//----------------------------------------------------------------
G4String GmDataPrimaryParticle::GetStringValueFromSecoTrack( const G4Track* aTrack1, const G4Track* )
{
  return aTrack1->GetDefinition()->GetParticleName();
}
