#include "GmDataInitialPVCopyNumber.hh"
#include "G4TransportationManager.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4VPhysicalVolume.hh"
#include "G4TouchableHistory.hh"
#include "G4TransportationManager.hh"

//----------------------------------------------------------------
GmDataInitialPVCopyNumber::GmDataInitialPVCopyNumber()
{
  bInitial = true;
  theExcludedTypes.clear(); // delete DTRun
}

//----------------------------------------------------------------
GmDataInitialPVCopyNumber::~GmDataInitialPVCopyNumber()
{
}


//----------------------------------------------------------------
G4double GmDataInitialPVCopyNumber::GetValueFromStep( const G4Step* aStep, G4int )
{

  return aStep->GetPreStepPoint()->GetTouchable()->GetReplicaNumber();

}

//----------------------------------------------------------------
G4double GmDataInitialPVCopyNumber::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  G4TouchableHistory* touch = new G4TouchableHistory;
  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndUpdateTouchable( aTrack->GetVertexPosition(), touch, false ); 

  G4int copyNo = touch->GetReplicaNumber();
 
  delete touch;

  return copyNo;
}

//----------------------------------------------------------------
G4double GmDataInitialPVCopyNumber::GetValueFromSecoTrack(const G4Track* aTrack, const G4Track* , G4int )
{

  G4TouchableHistory* touch = new G4TouchableHistory;
  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndUpdateTouchable( aTrack->GetVertexPosition(), touch, false ); 

  G4int copyNo = touch->GetReplicaNumber();
 
  delete touch;

  return copyNo;

}


//----------------------------------------------------------------
G4double GmDataInitialPVCopyNumber::GetValueFromEvent( const G4Event* anEvent, G4int )
{ 
  G4TouchableHistory* touch = new G4TouchableHistory;
  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndUpdateTouchable( anEvent->GetPrimaryVertex(0)->GetPosition(), touch, false ); 

  G4double val = touch->GetReplicaNumber();
 
  delete touch;

  return val;
}

//----------------------------------------------------------------
G4double GmDataInitialPVCopyNumber::GetValueFromStackedTrack( const G4Track* aTrack, G4int )
{
  G4TouchableHistory* touch = new G4TouchableHistory;
  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndUpdateTouchable( aTrack->GetPosition(), touch, false ); 

  G4int copyNo = touch->GetReplicaNumber();
 
  delete touch;

  return copyNo;
}
