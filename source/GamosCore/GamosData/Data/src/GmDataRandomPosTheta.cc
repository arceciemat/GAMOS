#include "GmDataRandomPosTheta.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "CLHEP/Random/RandFlat.h"

//----------------------------------------------------------------
GmDataRandomPosTheta::GmDataRandomPosTheta()
{
  bInitial = true;
  theHMin = 0.;
  theHMax = 180.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataRandomPosTheta::~GmDataRandomPosTheta()
{
}


//----------------------------------------------------------------
G4double GmDataRandomPosTheta::GetValueFromStep( const G4Step* aStep, G4int )
{
  G4double rand = CLHEP::RandFlat::shoot();
  return (aStep->GetPreStepPoint()->GetPosition() + rand*(aStep->GetPostStepPoint()->GetPosition()-aStep->GetPreStepPoint()->GetPosition())).theta()/CLHEP::deg;
}

//----------------------------------------------------------------
G4double GmDataRandomPosTheta::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  G4double rand = CLHEP::RandFlat::shoot();
  return (aTrack->GetVertexPosition()+rand*(aTrack->GetPosition()-aTrack->GetVertexPosition())).theta()/CLHEP::deg;
}
