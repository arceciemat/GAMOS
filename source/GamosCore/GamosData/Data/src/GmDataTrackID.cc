#include "GmDataTrackID.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmDataTrackID::GmDataTrackID()
{
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataTrackID::~GmDataTrackID()
{
}


//----------------------------------------------------------------
G4double GmDataTrackID::GetValueFromStep( const G4Step* aStep, G4int )
{

  return aStep->GetTrack()->GetTrackID();

}

//----------------------------------------------------------------
G4double GmDataTrackID::GetValueFromTrack( const G4Track* aTrack, G4int )
{

  return aTrack->GetTrackID();
}

//----------------------------------------------------------------
G4double GmDataTrackID::GetValueFromSecoTrack(const G4Track* , const G4Track* aTrack2, G4int )
{

  return aTrack2->GetTrackID();
}
