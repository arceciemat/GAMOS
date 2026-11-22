#include "GmDataParticleType.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataParticleType::GmDataParticleType()
{
  theNChar = 15;
  theNBytes = theNChar;

  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataParticleType::~GmDataParticleType()
{
}

 //----------------------------------------------------------------
G4String GmDataParticleType::GetStringValueFromStep( const G4Step* aStep )
{ 
  return aStep->GetTrack()->GetDefinition()->GetParticleType();
}

//----------------------------------------------------------------
G4String GmDataParticleType::GetStringValueFromTrack( const G4Track* aTrack )
{ 
  return aTrack->GetDefinition()->GetParticleType();
}

//----------------------------------------------------------------
G4String GmDataParticleType::GetStringValueFromSecoTrack( const G4Track* , const G4Track* aTrack2 )
{
  return aTrack2->GetDefinition()->GetParticleType();
}
