#include "GmDataPosChangeY.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataPosChangeY::GmDataPosChangeY()
{
  theHMin = -1000.;
  theHMax = 1000.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataPosChangeY::~GmDataPosChangeY()
{
}


//----------------------------------------------------------------
G4double GmDataPosChangeY::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPostStepPoint()->GetPosition().y()-aStep->GetPreStepPoint()->GetPosition().y();
}

//----------------------------------------------------------------
G4double GmDataPosChangeY::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetPosition().y()-aTrack->GetVertexPosition().y();
}
