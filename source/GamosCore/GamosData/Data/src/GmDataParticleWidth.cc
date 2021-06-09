#include "GmDataParticleWidth.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmDataParticleWidth::GmDataParticleWidth()
{
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataParticleWidth::~GmDataParticleWidth()
{
}


//----------------------------------------------------------------
G4double GmDataParticleWidth::GetValueFromStep( const G4Step* aStep, G4int )
{

  return aStep->GetTrack()->GetDefinition()->GetPDGWidth();

}

//----------------------------------------------------------------
G4double GmDataParticleWidth::GetValueFromTrack( const G4Track* aTrack, G4int )
{

  return aTrack->GetDefinition()->GetPDGWidth();
}

//----------------------------------------------------------------
G4double GmDataParticleWidth::GetValueFromSecoTrack(const G4Track*, const G4Track* aTrack2, G4int )
{

  return aTrack2->GetDefinition()->GetPDGWidth();
}
