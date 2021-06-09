#include "GmDataRandomPosPerp.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "CLHEP/Random/RandFlat.h"

//----------------------------------------------------------------
GmDataRandomPosPerp::GmDataRandomPosPerp()
{
  bInitial = true;
  theHMin = 0.;
  theHMax = 1000.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataRandomPosPerp::~GmDataRandomPosPerp()
{
}


//----------------------------------------------------------------
G4double GmDataRandomPosPerp::GetValueFromStep( const G4Step* aStep, G4int )
{
  G4double rand = CLHEP::RandFlat::shoot();
  return (aStep->GetPreStepPoint()->GetPosition() + rand*(aStep->GetPostStepPoint()->GetPosition()-aStep->GetPreStepPoint()->GetPosition())).perp();
}

//----------------------------------------------------------------
G4double GmDataRandomPosPerp::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  G4double rand = CLHEP::RandFlat::shoot();
  return (aTrack->GetVertexPosition()+rand*(aTrack->GetPosition()-aTrack->GetVertexPosition())).perp();
}

