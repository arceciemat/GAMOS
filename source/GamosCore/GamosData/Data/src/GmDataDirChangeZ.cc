#include "GmDataDirChangeZ.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataDirChangeZ::GmDataDirChangeZ()
{
  theHMin = -1.;
  theHMax = 1.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataDirChangeZ::~GmDataDirChangeZ()
{
}


//----------------------------------------------------------------
G4double GmDataDirChangeZ::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPostStepPoint()->GetMomentumDirection().z()-aStep->GetPreStepPoint()->GetMomentumDirection().z();
}

//----------------------------------------------------------------
G4double GmDataDirChangeZ::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetMomentumDirection().z()-aTrack->GetVertexMomentumDirection().z();
}
