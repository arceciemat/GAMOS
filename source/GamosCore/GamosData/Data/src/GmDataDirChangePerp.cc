#include "GmDataDirChangePerp.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataDirChangePerp::GmDataDirChangePerp()
{
  theHMin = 0.;
  theHMax = 1.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataDirChangePerp::~GmDataDirChangePerp()
{
}


//----------------------------------------------------------------
G4double GmDataDirChangePerp::GetValueFromStep( const G4Step* aStep, G4int )
{
  return (aStep->GetPostStepPoint()->GetMomentumDirection()-aStep->GetPreStepPoint()->GetMomentumDirection()).perp();
}

//----------------------------------------------------------------
G4double GmDataDirChangePerp::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return (aTrack->GetMomentumDirection()-aTrack->GetVertexMomentumDirection()).perp();
}
