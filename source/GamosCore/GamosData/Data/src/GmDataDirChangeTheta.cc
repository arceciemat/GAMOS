#include "GmDataDirChangeTheta.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataDirChangeTheta::GmDataDirChangeTheta()
{
  theHMin = 0.;
  theHMax = 180.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataDirChangeTheta::~GmDataDirChangeTheta()
{
}


//----------------------------------------------------------------
G4double GmDataDirChangeTheta::GetValueFromStep( const G4Step* aStep, G4int )
{
  return (aStep->GetPostStepPoint()->GetMomentumDirection()-aStep->GetPreStepPoint()->GetMomentumDirection()).theta()/CLHEP::deg;
}

//----------------------------------------------------------------
G4double GmDataDirChangeTheta::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return (aTrack->GetMomentumDirection()-aTrack->GetVertexMomentumDirection()).theta()/CLHEP::deg;
}
