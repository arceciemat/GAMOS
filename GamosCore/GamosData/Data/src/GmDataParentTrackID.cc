#include "GmDataParentTrackID.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmDataParentTrackID::GmDataParentTrackID()
{
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataParentTrackID::~GmDataParentTrackID()
{
}


//----------------------------------------------------------------
G4double GmDataParentTrackID::GetValueFromStep( const G4Step* aStep, G4int )
{

  return aStep->GetTrack()->GetParentID();

}

//----------------------------------------------------------------
G4double GmDataParentTrackID::GetValueFromTrack( const G4Track* aTrack, G4int )
{

  return aTrack->GetParentID();
}

//----------------------------------------------------------------
G4double GmDataParentTrackID::GetValueFromSecoTrack(const G4Track* aTrack1, const G4Track* , G4int )
{

  return aTrack1->GetTrackID();
}
