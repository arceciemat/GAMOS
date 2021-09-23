#include "GmDataInitialLocalPos1Y.hh"

#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4TouchableHistory.hh"
#include "G4TransportationManager.hh"

//----------------------------------------------------------------
GmDataInitialLocalPos1Y::GmDataInitialLocalPos1Y()
{
  bInitial = true;
  theHMin = -1000.;
  theHMax = 1000.;
}

//----------------------------------------------------------------
GmDataInitialLocalPos1Y::~GmDataInitialLocalPos1Y()
{
}


//----------------------------------------------------------------
G4double GmDataInitialLocalPos1Y::GetValueFromStep( const G4Step* aStep, G4int )
{
  G4ThreeVector localPos = GmG4Utils::GetLocalNFromGlobalPos( aStep->GetPreStepPoint()->GetPosition(), aStep->GetPreStepPoint()->GetTouchable()->GetHistory(), 1);

  return localPos.y();

}

//----------------------------------------------------------------
G4double GmDataInitialLocalPos1Y::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  G4ThreeVector pos = aTrack->GetVertexPosition();
  G4TouchableHistory* touch = new G4TouchableHistory;
  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndUpdateTouchable( pos, touch, false ); 

  G4ThreeVector localPos = GmG4Utils::GetLocalNFromGlobalPos( pos+G4ThreeVector(1.,0.,0.), touch->GetHistory(), 1 );
  delete touch;
  
  return localPos.y();
}

//----------------------------------------------------------------
G4double GmDataInitialLocalPos1Y::GetValueFromSecoTrack( const G4Track* aTrack1 , const G4Track* aTrack2, G4int )
{
  G4ThreeVector localPos = GmG4Utils::GetLocalNFromGlobalPos( aTrack2->GetPosition(), aTrack1->GetNextTouchable()->GetHistory(), 1 );

  return localPos.y();
}

//----------------------------------------------------------------
G4double GmDataInitialLocalPos1Y::GetValueFromEvent( const G4Event* anEvent, G4int )
{
  G4ThreeVector pos = anEvent->GetPrimaryVertex(0)->GetPosition();
  
  G4TouchableHistory* touch = new G4TouchableHistory;
  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndUpdateTouchable( pos , touch, false ); 

  G4ThreeVector localPos = GmG4Utils::GetLocalNFromGlobalPos( pos, touch->GetHistory(), 1 );
  delete touch;
  
  return localPos.y();
}

//----------------------------------------------------------------
G4double GmDataInitialLocalPos1Y::GetValueFromStackedTrack( const G4Track* aTrack, G4int )
{
  G4ThreeVector pos = aTrack->GetPosition();
  G4TouchableHistory* touch = new G4TouchableHistory;
  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndUpdateTouchable( pos, touch, false ); 

  G4ThreeVector localPos = GmG4Utils::GetLocalNFromGlobalPos( pos, touch->GetHistory(), 1 );
  delete touch;
  
  return localPos.y();
}
