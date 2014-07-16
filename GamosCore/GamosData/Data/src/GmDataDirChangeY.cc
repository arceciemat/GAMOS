#include "GmDataDirChangeY.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataDirChangeY::GmDataDirChangeY()
{
  theHMin = -1.;
  theHMax = 1.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataDirChangeY::~GmDataDirChangeY()
{
}


//----------------------------------------------------------------
G4double GmDataDirChangeY::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPostStepPoint()->GetMomentumDirection().y()-aStep->GetPreStepPoint()->GetMomentumDirection().y();
}

//----------------------------------------------------------------
G4double GmDataDirChangeY::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetMomentumDirection().y()-aTrack->GetVertexMomentumDirection().y();
}
