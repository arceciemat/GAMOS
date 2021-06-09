#include "GmDataFinalLocalPosTheta.hh"

#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4TouchableHistory.hh"
#include "G4TransportationManager.hh"

//----------------------------------------------------------------
GmDataFinalLocalPosTheta::GmDataFinalLocalPosTheta()
{
  bInitial = false;
  theHMin = 0.;
  theHMax = 180.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalLocalPosTheta::~GmDataFinalLocalPosTheta()
{
}


//----------------------------------------------------------------
G4double GmDataFinalLocalPosTheta::GetValueFromStep( const G4Step* aStep, G4int )
{
  G4ThreeVector localPos = GmG4Utils::GetLocalFromGlobalPos( aStep->GetPostStepPoint()->GetPosition(), aStep->GetPostStepPoint()->GetTouchable()->GetHistory());

  return localPos.theta()/CLHEP::deg;

}

//----------------------------------------------------------------
G4double GmDataFinalLocalPosTheta::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  G4ThreeVector localPos = GmG4Utils::GetLocalFromGlobalPos( aTrack->GetPosition(), aTrack->GetNextTouchable()->GetHistory());

  return localPos.theta()/CLHEP::deg;
}


