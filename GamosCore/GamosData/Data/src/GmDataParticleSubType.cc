#include "GmDataParticleSubType.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataParticleSubType::GmDataParticleSubType()
{
  theNChar = 15;
  theNBytes = theNChar;

  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataParticleSubType::~GmDataParticleSubType()
{
}

 //----------------------------------------------------------------
G4String GmDataParticleSubType::GetStringValueFromStep( const G4Step* aStep )
{ 
  return aStep->GetTrack()->GetDefinition()->GetParticleSubType();
}

//----------------------------------------------------------------
G4String GmDataParticleSubType::GetStringValueFromTrack( const G4Track* aTrack )
{ 
  return aTrack->GetDefinition()->GetParticleSubType();
}

//----------------------------------------------------------------
G4String GmDataParticleSubType::GetStringValueFromSecoTrack( const G4Track* , const G4Track* aTrack2 )
{
  return aTrack2->GetDefinition()->GetParticleSubType();
}
