#include "GmDataVertexLocalPos1Z.hh"

#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4TouchableHistory.hh"
#include "G4TransportationManager.hh"

//----------------------------------------------------------------
GmDataVertexLocalPos1Z::GmDataVertexLocalPos1Z()
{
  bInitial = true;
  theHMin = -1000.;
  theHMax = 1000.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataVertexLocalPos1Z::~GmDataVertexLocalPos1Z()
{
}

//----------------------------------------------------------------
G4double GmDataVertexLocalPos1Z::GetValueFromStep( const G4Step* aStep, G4int )
{
  G4ThreeVector pos = aStep->GetTrack()->GetVertexPosition();
  G4TouchableHistory* touch = new G4TouchableHistory;
  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndUpdateTouchable( pos, touch, false ); 

  G4ThreeVector localPos = GmG4Utils::GetLocalNFromGlobalPos( pos, touch->GetHistory(), 1 );
  delete touch;
  
  return localPos.z();

}

//----------------------------------------------------------------
G4double GmDataVertexLocalPos1Z::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  G4ThreeVector pos = aTrack->GetVertexPosition();
  G4TouchableHistory* touch = new G4TouchableHistory;
  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndUpdateTouchable( pos, touch, false ); 

  G4ThreeVector localPos = GmG4Utils::GetLocalNFromGlobalPos( pos, touch->GetHistory(), 1 );
  delete touch;
  
  return localPos.z();
}

//----------------------------------------------------------------
G4double GmDataVertexLocalPos1Z::GetValueFromStackedTrack( const G4Track* aTrack, G4int )
{
  G4ThreeVector pos = aTrack->GetVertexPosition();
  G4TouchableHistory* touch = new G4TouchableHistory;
  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndUpdateTouchable( pos, touch, false ); 

  G4ThreeVector localPos = GmG4Utils::GetLocalNFromGlobalPos( pos, touch->GetHistory(), 1 );
  delete touch;
  
  return localPos.z();
}
