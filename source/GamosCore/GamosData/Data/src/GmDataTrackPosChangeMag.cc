#include "GmDataTrackPosChangeMag.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataTrackPosChangeMag::GmDataTrackPosChangeMag()
{
  theHMin = 0.;
  theHMax = 1000.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataTrackPosChangeMag::~GmDataTrackPosChangeMag()
{
}


//----------------------------------------------------------------
G4double GmDataTrackPosChangeMag::GetValueFromStep( const G4Step* aStep, G4int )
{
  return (aStep->GetPostStepPoint()->GetPosition()-aStep->GetTrack()->GetVertexPosition()).mag();
}

//----------------------------------------------------------------
G4double GmDataTrackPosChangeMag::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return (aTrack->GetPosition()-aTrack->GetVertexPosition()).mag();
}
