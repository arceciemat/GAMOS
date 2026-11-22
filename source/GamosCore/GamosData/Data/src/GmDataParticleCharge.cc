#include "GmDataParticleCharge.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmDataParticleCharge::GmDataParticleCharge()
{
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataParticleCharge::~GmDataParticleCharge()
{
}


//----------------------------------------------------------------
G4double GmDataParticleCharge::GetValueFromStep( const G4Step* aStep, G4int )
{

  return aStep->GetTrack()->GetDefinition()->GetPDGCharge();

}

//----------------------------------------------------------------
G4double GmDataParticleCharge::GetValueFromTrack( const G4Track* aTrack, G4int )
{

  return aTrack->GetDefinition()->GetPDGCharge();
}

//----------------------------------------------------------------
G4double GmDataParticleCharge::GetValueFromSecoTrack(const G4Track*, const G4Track* aTrack2, G4int )
{

  return aTrack2->GetDefinition()->GetPDGCharge();
}
