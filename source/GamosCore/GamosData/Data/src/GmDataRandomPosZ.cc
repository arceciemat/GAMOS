#include "GmDataRandomPosZ.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "CLHEP/Random/RandFlat.h"

//----------------------------------------------------------------
GmDataRandomPosZ::GmDataRandomPosZ()
{
  bInitial = true;
  theHMin = -1000.;
  theHMax = 1000.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataRandomPosZ::~GmDataRandomPosZ()
{
}


//----------------------------------------------------------------
G4double GmDataRandomPosZ::GetValueFromStep( const G4Step* aStep, G4int )
{
  G4double rand = CLHEP::RandFlat::shoot();
  return aStep->GetPreStepPoint()->GetPosition().z() + rand*(aStep->GetPostStepPoint()->GetPosition().z()-aStep->GetPreStepPoint()->GetPosition().z());
}

//----------------------------------------------------------------
G4double GmDataRandomPosZ::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  G4double rand = CLHEP::RandFlat::shoot();
  return aTrack->GetVertexPosition().z()+rand*(aTrack->GetPosition().z()-aTrack->GetVertexPosition().z());

}
