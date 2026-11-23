#include "GmDataParticle.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataParticle::GmDataParticle()
{
  theNChar = 15;
  theNBytes = theNChar;

  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataParticle::~GmDataParticle()
{
}

 //----------------------------------------------------------------
G4String GmDataParticle::GetStringValueFromStep( const G4Step* aStep )
{ 
  return aStep->GetTrack()->GetDefinition()->GetParticleName();
}

//----------------------------------------------------------------
G4String GmDataParticle::GetStringValueFromTrack( const G4Track* aTrack )
{ 
  return aTrack->GetDefinition()->GetParticleName();
}

//----------------------------------------------------------------
G4String GmDataParticle::GetStringValueFromSecoTrack( const G4Track* , const G4Track* aTrack2 )
{
  return aTrack2->GetDefinition()->GetParticleName();
}
