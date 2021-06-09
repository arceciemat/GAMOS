#include "GmDataPosChangeX.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataPosChangeX::GmDataPosChangeX()
{
  theHMin = -1000.;
  theHMax = 1000.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataPosChangeX::~GmDataPosChangeX()
{
}


//----------------------------------------------------------------
G4double GmDataPosChangeX::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPostStepPoint()->GetPosition().x()-aStep->GetPreStepPoint()->GetPosition().x();
}

//----------------------------------------------------------------
G4double GmDataPosChangeX::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetPosition().x()-aTrack->GetVertexPosition().x();
}
