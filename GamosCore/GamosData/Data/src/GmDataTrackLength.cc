#include "GmDataTrackLength.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmDataTrackLength::GmDataTrackLength()
{
  theHMin = 0.;
  theHMax = 1000.;
  theExcludedTypes.insert( DTEvent );
}

//----------------------------------------------------------------
GmDataTrackLength::~GmDataTrackLength()
{
}


//----------------------------------------------------------------
G4double GmDataTrackLength::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetTrack()->GetTrackLength();
}

//----------------------------------------------------------------
G4double GmDataTrackLength::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetTrackLength();
}


G4double GmDataTrackLength::GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track* , G4int )
{
  return aTrack1->GetTrackLength();
}

