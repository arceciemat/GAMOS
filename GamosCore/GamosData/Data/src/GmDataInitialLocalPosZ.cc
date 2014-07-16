#include "GmDataInitialLocalPosZ.hh"

#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4TouchableHistory.hh"
#include "G4TransportationManager.hh"

//----------------------------------------------------------------
GmDataInitialLocalPosZ::GmDataInitialLocalPosZ()
{
  bInitial = true;
  theHMin = -1000.;
  theHMax = 1000.;
}

//----------------------------------------------------------------
GmDataInitialLocalPosZ::~GmDataInitialLocalPosZ()
{
}


//----------------------------------------------------------------
G4double GmDataInitialLocalPosZ::GetValueFromStep( const G4Step* aStep, G4int )
{
  G4ThreeVector localPos = GmG4Utils::GetLocalFromGlobalPos( aStep->GetPreStepPoint()->GetPosition(), aStep->GetPreStepPoint()->GetTouchable()->GetHistory());

  return localPos.z();

}

//----------------------------------------------------------------
G4double GmDataInitialLocalPosZ::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  G4ThreeVector pos = aTrack->GetVertexPosition();
  G4TouchableHistory* touch = new G4TouchableHistory;
  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndUpdateTouchable( pos, touch, false ); 

  G4ThreeVector localPos = GmG4Utils::GetLocalFromGlobalPos( pos, touch->GetHistory() );

  return localPos.z();
}

//----------------------------------------------------------------
G4double GmDataInitialLocalPosZ::GetValueFromSecoTrack( const G4Track* aTrack1 , const G4Track* aTrack2, G4int )
{
  G4ThreeVector localPos = GmG4Utils::GetLocalFromGlobalPos( aTrack2->GetPosition(), aTrack1->GetNextTouchable()->GetHistory() );

  return localPos.z();
}

//----------------------------------------------------------------
G4double GmDataInitialLocalPosZ::GetValueFromEvent( const G4Event* anEvent, G4int )
{
  G4ThreeVector pos = anEvent->GetPrimaryVertex(0)->GetPosition();
  
  G4TouchableHistory* touch = new G4TouchableHistory;
  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndUpdateTouchable( pos , touch, false ); 

  G4ThreeVector localPos = GmG4Utils::GetLocalFromGlobalPos( pos, touch->GetHistory() );

  return localPos.z();
}

