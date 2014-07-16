#include "GmDataPosChangeTheta.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataPosChangeTheta::GmDataPosChangeTheta()
{
  theHMin = 0.;
  theHMax = 180.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataPosChangeTheta::~GmDataPosChangeTheta()
{
}


//----------------------------------------------------------------
G4double GmDataPosChangeTheta::GetValueFromStep( const G4Step* aStep, G4int )
{
  return (aStep->GetPostStepPoint()->GetPosition()-aStep->GetPreStepPoint()->GetPosition()).theta()/CLHEP::deg;
}

//----------------------------------------------------------------
G4double GmDataPosChangeTheta::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return (aTrack->GetPosition()-aTrack->GetVertexPosition()).theta()/CLHEP::deg;
}
