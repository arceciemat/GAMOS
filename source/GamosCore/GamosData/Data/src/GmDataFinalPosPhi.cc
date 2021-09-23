#include "GmDataFinalPosPhi.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataFinalPosPhi::GmDataFinalPosPhi()
{
  bInitial = false;
  theHMin = -180.;
  theHMax = 180.;
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalPosPhi::~GmDataFinalPosPhi()
{
}


//----------------------------------------------------------------
G4double GmDataFinalPosPhi::GetValueFromStep( const G4Step* aStep, G4int )
{
  //  G4cout << " GmDataFinalPosPhi::GetValueFromStep " << aStep->GetPostStepPoint()->GetPosition().phi()/CLHEP::deg << " " << aStep->GetPostStepPoint()->GetPosition() << G4endl;
  return aStep->GetPostStepPoint()->GetPosition().phi()/CLHEP::deg;
}

//----------------------------------------------------------------
G4double GmDataFinalPosPhi::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetPosition().phi()/CLHEP::deg;
}

//----------------------------------------------------------------
G4double GmDataFinalPosPhi::GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track*, G4int )
{
  return aTrack1->GetPosition().phi()/CLHEP::deg;
}
