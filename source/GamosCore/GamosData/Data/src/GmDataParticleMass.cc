#include "GmDataParticleMass.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmDataParticleMass::GmDataParticleMass()
{
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataParticleMass::~GmDataParticleMass()
{
}


//----------------------------------------------------------------
G4double GmDataParticleMass::GetValueFromStep( const G4Step* aStep, G4int )
{

  return aStep->GetTrack()->GetDefinition()->GetPDGMass();

}

//----------------------------------------------------------------
G4double GmDataParticleMass::GetValueFromTrack( const G4Track* aTrack, G4int )
{

  return aTrack->GetDefinition()->GetPDGMass();
}

//----------------------------------------------------------------
G4double GmDataParticleMass::GetValueFromSecoTrack(const G4Track*, const G4Track* aTrack2, G4int )
{

  return aTrack2->GetDefinition()->GetPDGMass();
}
