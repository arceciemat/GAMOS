#include "GmDataRandomPosX.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "CLHEP/Random/RandFlat.h"

//----------------------------------------------------------------
GmDataRandomPosX::GmDataRandomPosX()
{
  bInitial = true;
  theHMin = -1000.;
  theHMax = 1000.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataRandomPosX::~GmDataRandomPosX()
{
}


//----------------------------------------------------------------
G4double GmDataRandomPosX::GetValueFromStep( const G4Step* aStep, G4int )
{
  G4double rand = CLHEP::RandFlat::shoot();
  return aStep->GetPreStepPoint()->GetPosition().x() + rand*(aStep->GetPostStepPoint()->GetPosition().x()-aStep->GetPreStepPoint()->GetPosition().x());
}

//----------------------------------------------------------------
G4double GmDataRandomPosX::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  G4double rand = CLHEP::RandFlat::shoot();
  return aTrack->GetVertexPosition().x()+rand*(aTrack->GetPosition().x()-aTrack->GetVertexPosition().x());
}
