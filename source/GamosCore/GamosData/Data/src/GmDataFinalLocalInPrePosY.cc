#include "GmDataFinalLocalInPrePosY.hh"

#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4TouchableHistory.hh"
#include "G4TransportationManager.hh"

//----------------------------------------------------------------
GmDataFinalLocalInPrePosY::GmDataFinalLocalInPrePosY()
{
  bInitial = false;
  theHMin = -1000.;
  theHMax = 1000.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalLocalInPrePosY::~GmDataFinalLocalInPrePosY()
{
}


//----------------------------------------------------------------
G4double GmDataFinalLocalInPrePosY::GetValueFromStep( const G4Step* aStep, G4int )
{
  G4ThreeVector localPos = GmG4Utils::GetLocalFromGlobalPos( aStep->GetPostStepPoint()->GetPosition(), aStep->GetPreStepPoint()->GetTouchable()->GetHistory());

  return localPos.y();

}

//----------------------------------------------------------------
G4double GmDataFinalLocalInPrePosY::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  G4ThreeVector pos = aTrack->GetVertexPosition();
  G4TouchableHistory* touch = new G4TouchableHistory;
  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndUpdateTouchable( pos, touch, false ); 

  G4ThreeVector localPos = GmG4Utils::GetLocalFromGlobalPos( aTrack->GetPosition(), touch->GetHistory() );

  return localPos.y();
}


