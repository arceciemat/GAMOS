#include "GmDataInitialLocalPosPhi.hh"

#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4TouchableHistory.hh"
#include "G4TransportationManager.hh"

//----------------------------------------------------------------
GmDataInitialLocalPosPhi::GmDataInitialLocalPosPhi()
{
  bInitial = true;
  theHMin = -180.;
  theHMax = 180.;
}

//----------------------------------------------------------------
GmDataInitialLocalPosPhi::~GmDataInitialLocalPosPhi()
{
}


//----------------------------------------------------------------
G4double GmDataInitialLocalPosPhi::GetValueFromStep( const G4Step* aStep, G4int )
{
  G4ThreeVector localPos = GmG4Utils::GetLocalFromGlobalPos( aStep->GetPreStepPoint()->GetPosition(), aStep->GetPreStepPoint()->GetTouchable()->GetHistory());

  return localPos.phi()/CLHEP::deg;

}

//----------------------------------------------------------------
G4double GmDataInitialLocalPosPhi::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  G4ThreeVector pos = aTrack->GetVertexPosition();
  G4TouchableHistory* touch = new G4TouchableHistory;
  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndUpdateTouchable( pos, touch, false ); 

  G4ThreeVector localPos = GmG4Utils::GetLocalFromGlobalPos( pos, touch->GetHistory() );

  return localPos.phi()/CLHEP::deg;
}

//----------------------------------------------------------------
G4double GmDataInitialLocalPosPhi::GetValueFromSecoTrack( const G4Track* aTrack1 , const G4Track* aTrack2, G4int )
{
  G4ThreeVector localPos = GmG4Utils::GetLocalFromGlobalPos( aTrack2->GetPosition(), aTrack1->GetNextTouchable()->GetHistory() );

  return localPos.phi()/CLHEP::deg;
}

//----------------------------------------------------------------
G4double GmDataInitialLocalPosPhi::GetValueFromEvent( const G4Event* anEvent, G4int )
{
  G4ThreeVector pos = anEvent->GetPrimaryVertex(0)->GetPosition();
  
  G4TouchableHistory* touch = new G4TouchableHistory;
  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndUpdateTouchable( pos , touch, false ); 

  G4ThreeVector localPos = GmG4Utils::GetLocalFromGlobalPos( pos, touch->GetHistory() );

  return localPos.phi()/CLHEP::deg;
}

