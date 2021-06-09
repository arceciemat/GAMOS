#include "GmDataInitialLocalDirZ.hh"

#include "GamosCore/GamosUtils/include/GmG4Utils.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4TouchableHistory.hh"
#include "G4TransportationManager.hh"

//----------------------------------------------------------------
GmDataInitialLocalDirZ::GmDataInitialLocalDirZ()
{
  bInitial = true;
  theHMin = 0.;
  theHMax = 1.;
}

//----------------------------------------------------------------
GmDataInitialLocalDirZ::~GmDataInitialLocalDirZ()
{
}


//----------------------------------------------------------------
G4double GmDataInitialLocalDirZ::GetValueFromStep( const G4Step* aStep, G4int )
{
  G4ThreeVector localDir = GmG4Utils::GetLocalFromGlobalDir( aStep->GetPreStepPoint()->GetMomentumDirection(), aStep->GetPreStepPoint()->GetTouchable()->GetHistory());

  return localDir.z();

}

//----------------------------------------------------------------
G4double GmDataInitialLocalDirZ::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  G4ThreeVector pos = aTrack->GetVertexPosition();
  G4ThreeVector dir = aTrack->GetVertexMomentumDirection();
  G4TouchableHistory* touch = new G4TouchableHistory;
  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndUpdateTouchable( pos, touch, false ); 

  G4ThreeVector localDir = GmG4Utils::GetLocalFromGlobalDir( dir, touch->GetHistory() );

  return localDir.z();
}

//----------------------------------------------------------------
G4double GmDataInitialLocalDirZ::GetValueFromSecoTrack( const G4Track* aTrack1 , const G4Track* aTrack2, G4int )
{
  G4ThreeVector localDir = GmG4Utils::GetLocalFromGlobalDir( aTrack2->GetMomentumDirection(), aTrack1->GetNextTouchable()->GetHistory() );

  return localDir.z();
}

//----------------------------------------------------------------
G4double GmDataInitialLocalDirZ::GetValueFromEvent( const G4Event* anEvent, G4int )
{
  G4ThreeVector pos = anEvent->GetPrimaryVertex(0)->GetPosition();

  G4ThreeVector dir = anEvent->GetPrimaryVertex(0)->GetPrimary(0)->GetMomentum().unit();
  
  G4TouchableHistory* touch = new G4TouchableHistory;
  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndUpdateTouchable( pos , touch, false ); 

  G4ThreeVector localDir = GmG4Utils::GetLocalFromGlobalDir( dir, touch->GetHistory() );

  return localDir.z();
}

