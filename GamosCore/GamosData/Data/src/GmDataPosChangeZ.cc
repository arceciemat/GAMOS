#include "GmDataPosChangeZ.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataPosChangeZ::GmDataPosChangeZ()
{
  theHMin = -1000.;
  theHMax = 1000.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataPosChangeZ::~GmDataPosChangeZ()
{
}


//----------------------------------------------------------------
G4double GmDataPosChangeZ::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPostStepPoint()->GetPosition().z()-aStep->GetPreStepPoint()->GetPosition().z();
}

//----------------------------------------------------------------
G4double GmDataPosChangeZ::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetPosition().z()-aTrack->GetVertexPosition().z();
}
