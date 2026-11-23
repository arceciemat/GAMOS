#include "GmDataPosChangePerp.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataPosChangePerp::GmDataPosChangePerp()
{
  theHMin = 0.;
  theHMax = 1000.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataPosChangePerp::~GmDataPosChangePerp()
{
}


//----------------------------------------------------------------
G4double GmDataPosChangePerp::GetValueFromStep( const G4Step* aStep, G4int )
{
  return (aStep->GetPostStepPoint()->GetPosition()-aStep->GetPreStepPoint()->GetPosition()).perp();
}

//----------------------------------------------------------------
G4double GmDataPosChangePerp::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return (aTrack->GetPosition()-aTrack->GetVertexPosition()).perp();
}
