#include "GmDataFinalPosTheta.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataFinalPosTheta::GmDataFinalPosTheta()
{
  bInitial = false;
  theHMin = 0.;
  theHMax = 180.;
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalPosTheta::~GmDataFinalPosTheta()
{
}


//----------------------------------------------------------------
G4double GmDataFinalPosTheta::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPostStepPoint()->GetPosition().theta()/CLHEP::deg;
}

//----------------------------------------------------------------
G4double GmDataFinalPosTheta::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetPosition().theta()/CLHEP::deg;
}

//----------------------------------------------------------------
G4double GmDataFinalPosTheta::GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track*, G4int )
{
  return aTrack1->GetPosition().theta()/CLHEP::deg;
}
