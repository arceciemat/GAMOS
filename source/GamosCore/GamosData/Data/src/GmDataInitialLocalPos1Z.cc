#include "GmDataInitialLocalPos1Z.hh"

#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4TouchableHistory.hh"
#include "G4TransportationManager.hh"

//----------------------------------------------------------------
GmDataInitialLocalPos1Z::GmDataInitialLocalPos1Z()
{
  bInitial = true;
  theHMin = -1000.;
  theHMax = 1000.;
}

//----------------------------------------------------------------
GmDataInitialLocalPos1Z::~GmDataInitialLocalPos1Z()
{
}


//----------------------------------------------------------------
G4double GmDataInitialLocalPos1Z::GetValueFromStep( const G4Step* aStep, G4int )
{
  G4ThreeVector localPos = GmG4Utils::GetLocalNFromGlobalPos( aStep->GetPreStepPoint()->GetPosition(), aStep->GetPreStepPoint()->GetTouchable()->GetHistory(), 1);

  return localPos.z();

}

//----------------------------------------------------------------
G4double GmDataInitialLocalPos1Z::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  G4ThreeVector pos = aTrack->GetVertexPosition();
  G4TouchableHistory* touch = new G4TouchableHistory;
  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndUpdateTouchable( pos, touch, false ); 

  G4ThreeVector localPos = GmG4Utils::GetLocalNFromGlobalPos( pos+G4ThreeVector(1.,0.,0.), touch->GetHistory(), 1 );
  delete touch;
  
  return localPos.z();
}

//----------------------------------------------------------------
G4double GmDataInitialLocalPos1Z::GetValueFromSecoTrack( const G4Track* aTrack1 , const G4Track* aTrack2, G4int )
{
  G4ThreeVector localPos = GmG4Utils::GetLocalNFromGlobalPos( aTrack2->GetPosition(), aTrack1->GetNextTouchable()->GetHistory(), 1 );

  return localPos.z();
}

//----------------------------------------------------------------
G4double GmDataInitialLocalPos1Z::GetValueFromEvent( const G4Event* anEvent, G4int )
{
  G4ThreeVector pos = anEvent->GetPrimaryVertex(0)->GetPosition();
  
  G4TouchableHistory* touch = new G4TouchableHistory;
  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndUpdateTouchable( pos , touch, false ); 

  G4ThreeVector localPos = GmG4Utils::GetLocalNFromGlobalPos( pos, touch->GetHistory(), 1 );
  delete touch;
  
  return localPos.z();
}

//----------------------------------------------------------------
G4double GmDataInitialLocalPos1Z::GetValueFromStackedTrack( const G4Track* aTrack, G4int )
{
  G4ThreeVector pos = aTrack->GetPosition();
  G4TouchableHistory* touch = new G4TouchableHistory;
  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndUpdateTouchable( pos, touch, false ); 

  G4ThreeVector localPos = GmG4Utils::GetLocalNFromGlobalPos( pos, touch->GetHistory(), 1 );
  delete touch;
  
  return localPos.z();
}
