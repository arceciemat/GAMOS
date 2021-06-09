#include "GmDataParticlePDGEncoding.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmDataParticlePDGEncoding::GmDataParticlePDGEncoding()
{
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataParticlePDGEncoding::~GmDataParticlePDGEncoding()
{
}


//----------------------------------------------------------------
G4double GmDataParticlePDGEncoding::GetValueFromStep( const G4Step* aStep, G4int )
{

  return aStep->GetTrack()->GetDefinition()->GetPDGEncoding();

}

//----------------------------------------------------------------
G4double GmDataParticlePDGEncoding::GetValueFromTrack( const G4Track* aTrack, G4int )
{

  return aTrack->GetDefinition()->GetPDGEncoding();
}

//----------------------------------------------------------------
G4double GmDataParticlePDGEncoding::GetValueFromSecoTrack(const G4Track*, const G4Track* aTrack2, G4int )
{

  return aTrack2->GetDefinition()->GetPDGEncoding();
}
