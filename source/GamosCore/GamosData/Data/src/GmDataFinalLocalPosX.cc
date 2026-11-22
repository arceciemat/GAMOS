#include "GmDataFinalLocalPosX.hh"

#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4TouchableHistory.hh"
#include "G4TransportationManager.hh"

//----------------------------------------------------------------
GmDataFinalLocalPosX::GmDataFinalLocalPosX()
{
  bInitial = false;
  theHMin = -1000.;
  theHMax = 1000.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalLocalPosX::~GmDataFinalLocalPosX()
{
}


//----------------------------------------------------------------
G4double GmDataFinalLocalPosX::GetValueFromStep( const G4Step* aStep, G4int )
{
  G4ThreeVector localPos = GmG4Utils::GetLocalFromGlobalPos( aStep->GetPostStepPoint()->GetPosition(), aStep->GetPostStepPoint()->GetTouchable()->GetHistory());

  return localPos.x();

}

//----------------------------------------------------------------
G4double GmDataFinalLocalPosX::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  G4ThreeVector localPos = GmG4Utils::GetLocalFromGlobalPos( aTrack->GetPosition(), aTrack->GetNextTouchable()->GetHistory());

  return localPos.x();
}


