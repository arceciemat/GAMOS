#include "GmDataPosChangeMag.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataPosChangeMag::GmDataPosChangeMag()
{
  theHMin = 0.;
  theHMax = 1000.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataPosChangeMag::~GmDataPosChangeMag()
{
}


//----------------------------------------------------------------
G4double GmDataPosChangeMag::GetValueFromStep( const G4Step* aStep, G4int )
{
  return (aStep->GetPostStepPoint()->GetPosition()-aStep->GetPreStepPoint()->GetPosition()).mag();
}

//----------------------------------------------------------------
G4double GmDataPosChangeMag::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return (aTrack->GetPosition()-aTrack->GetVertexPosition()).mag();
}
