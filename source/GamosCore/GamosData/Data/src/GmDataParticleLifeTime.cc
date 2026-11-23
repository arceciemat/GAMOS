#include "GmDataParticleLifeTime.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmDataParticleLifeTime::GmDataParticleLifeTime()
{
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataParticleLifeTime::~GmDataParticleLifeTime()
{
}


//----------------------------------------------------------------
G4double GmDataParticleLifeTime::GetValueFromStep( const G4Step* aStep, G4int )
{

  return aStep->GetTrack()->GetDefinition()->GetPDGLifeTime();

}

//----------------------------------------------------------------
G4double GmDataParticleLifeTime::GetValueFromTrack( const G4Track* aTrack, G4int )
{

  return aTrack->GetDefinition()->GetPDGLifeTime();
}

//----------------------------------------------------------------
G4double GmDataParticleLifeTime::GetValueFromSecoTrack(const G4Track*, const G4Track* aTrack2, G4int )
{

  return aTrack2->GetDefinition()->GetPDGLifeTime();
}
