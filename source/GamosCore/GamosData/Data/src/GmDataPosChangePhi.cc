#include "GmDataPosChangePhi.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataPosChangePhi::GmDataPosChangePhi()
{
  theHMin = -180.;
  theHMax = 180.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataPosChangePhi::~GmDataPosChangePhi()
{
}


//----------------------------------------------------------------
G4double GmDataPosChangePhi::GetValueFromStep( const G4Step* aStep, G4int )
{
  return (aStep->GetPostStepPoint()->GetPosition()-aStep->GetPreStepPoint()->GetPosition()).phi()/CLHEP::deg;
}

//----------------------------------------------------------------
G4double GmDataPosChangePhi::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return (aTrack->GetPosition()-aTrack->GetVertexPosition()).phi()/CLHEP::deg;
}
