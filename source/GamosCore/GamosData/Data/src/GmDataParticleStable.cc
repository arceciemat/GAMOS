#include "GmDataParticleStable.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmDataParticleStable::GmDataParticleStable()
{
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataParticleStable::~GmDataParticleStable()
{
}


//----------------------------------------------------------------
G4double GmDataParticleStable::GetValueFromStep( const G4Step* aStep, G4int )
{

  return aStep->GetTrack()->GetDefinition()->GetPDGStable();

}

//----------------------------------------------------------------
G4double GmDataParticleStable::GetValueFromTrack( const G4Track* aTrack, G4int )
{

  return aTrack->GetDefinition()->GetPDGStable();
}

//----------------------------------------------------------------
G4double GmDataParticleStable::GetValueFromSecoTrack(const G4Track*, const G4Track* aTrack2, G4int )
{

  return aTrack2->GetDefinition()->GetPDGStable();
}
