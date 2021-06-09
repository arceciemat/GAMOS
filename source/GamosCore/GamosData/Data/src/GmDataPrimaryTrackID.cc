#include "GmDataPrimaryTrackID.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmDataPrimaryTrackID::GmDataPrimaryTrackID()
{
  theExcludedTypes.clear(); // delete DTRun and DTEvent
  theExcludedTypes.insert(DTEvent);
  theExcludedTypes.insert(DTRun);
}

//----------------------------------------------------------------
GmDataPrimaryTrackID::~GmDataPrimaryTrackID()
{
}


//----------------------------------------------------------------
G4double GmDataPrimaryTrackID::GetValueFromStep( const G4Step* aStep, G4int )
{

  return aStep->GetTrack()->GetTrackID();

}

//----------------------------------------------------------------
G4double GmDataPrimaryTrackID::GetValueFromTrack( const G4Track* aTrack, G4int )
{

  return aTrack->GetTrackID();
}

//----------------------------------------------------------------
G4double GmDataPrimaryTrackID::GetValueFromSecoTrack(const G4Track* aTrack1, const G4Track*, G4int )
{

  return aTrack1->GetTrackID();
}
