#include "GmDataRandomPosY.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "CLHEP/Random/RandFlat.h"

//----------------------------------------------------------------
GmDataRandomPosY::GmDataRandomPosY()
{
  bInitial = true;
  theHMin = -1000.;
  theHMax = 1000.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataRandomPosY::~GmDataRandomPosY()
{
}


//----------------------------------------------------------------
G4double GmDataRandomPosY::GetValueFromStep( const G4Step* aStep, G4int )
{
  G4double rand = CLHEP::RandFlat::shoot();
  return aStep->GetPreStepPoint()->GetPosition().y() + rand*(aStep->GetPostStepPoint()->GetPosition().y()-aStep->GetPreStepPoint()->GetPosition().y());

}

//----------------------------------------------------------------
G4double GmDataRandomPosY::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  G4double rand = CLHEP::RandFlat::shoot();
  return aTrack->GetVertexPosition().y()+rand*(aTrack->GetPosition().y()-aTrack->GetVertexPosition().y());

}
