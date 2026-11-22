#include "GmDataRandomPosPhi.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "CLHEP/Random/RandFlat.h"

//----------------------------------------------------------------
GmDataRandomPosPhi::GmDataRandomPosPhi()
{
  bInitial = true;
  theHMin = -180.;
  theHMax = 180.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataRandomPosPhi::~GmDataRandomPosPhi()
{
}


//----------------------------------------------------------------
G4double GmDataRandomPosPhi::GetValueFromStep( const G4Step* aStep, G4int )
{
  G4double rand = CLHEP::RandFlat::shoot();
  //  G4cout << " GmDataInitalPosPhi::GetValueFromStep " << aStep->GetPreStepPoint()->GetPosition().phi()/CLHEP::deg << " " << aStep->GetPreStepPoint()->GetPosition() << G4endl;
  return (aStep->GetPreStepPoint()->GetPosition() + rand*(aStep->GetPostStepPoint()->GetPosition()-aStep->GetPreStepPoint()->GetPosition())).phi()/CLHEP::deg;
}

//----------------------------------------------------------------
G4double GmDataRandomPosPhi::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  G4double rand = CLHEP::RandFlat::shoot();
  return (aTrack->GetVertexPosition()+rand*(aTrack->GetPosition()-aTrack->GetVertexPosition())).phi()/CLHEP::deg;
}

