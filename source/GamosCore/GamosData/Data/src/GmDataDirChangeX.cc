#include "GmDataDirChangeX.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataDirChangeX::GmDataDirChangeX()
{
  theHMin = -1.;
  theHMax = 1.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataDirChangeX::~GmDataDirChangeX()
{
}


//----------------------------------------------------------------
G4double GmDataDirChangeX::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPostStepPoint()->GetMomentumDirection().x()-aStep->GetPreStepPoint()->GetMomentumDirection().x();
}

//----------------------------------------------------------------
G4double GmDataDirChangeX::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetMomentumDirection().x()-aTrack->GetVertexMomentumDirection().x();
}
