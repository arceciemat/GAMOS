#include "GmDataFinalLocalPosY.hh"

#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4TouchableHistory.hh"
#include "G4TransportationManager.hh"

//----------------------------------------------------------------
GmDataFinalLocalPosY::GmDataFinalLocalPosY()
{
  bInitial = false;
  theHMin = -1000.;
  theHMax = 1000.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalLocalPosY::~GmDataFinalLocalPosY()
{
}


//----------------------------------------------------------------
G4double GmDataFinalLocalPosY::GetValueFromStep( const G4Step* aStep, G4int )
{
  G4ThreeVector localPos = GmG4Utils::GetLocalFromGlobalPos( aStep->GetPostStepPoint()->GetPosition(), aStep->GetPostStepPoint()->GetTouchable()->GetHistory());

  return localPos.y();

}

//----------------------------------------------------------------
G4double GmDataFinalLocalPosY::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  G4ThreeVector localPos = GmG4Utils::GetLocalFromGlobalPos( aTrack->GetPosition(), aTrack->GetNextTouchable()->GetHistory());

  return localPos.y();
}


