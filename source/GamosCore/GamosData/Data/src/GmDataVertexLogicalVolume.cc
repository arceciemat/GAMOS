#include "GmDataVertexLogicalVolume.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4TouchableHistory.hh"
#include "G4TransportationManager.hh"

//----------------------------------------------------------------
GmDataVertexLogicalVolume::GmDataVertexLogicalVolume()
{
  bInitial = true;
  theNChar = 25;
  theNBytes = theNChar;

  //  theExcludedTypes.insert(DTSeco);
}

//----------------------------------------------------------------
GmDataVertexLogicalVolume::~GmDataVertexLogicalVolume()
{
}

//----------------------------------------------------------------
G4String GmDataVertexLogicalVolume::GetStringValueFromStep( const G4Step* aStep )
{ 
  return aStep->GetTrack()->GetLogicalVolumeAtVertex()->GetName();
}

//----------------------------------------------------------------
G4String GmDataVertexLogicalVolume::GetStringValueFromTrack( const G4Track* aTrack )
{ 
  return aTrack->GetLogicalVolumeAtVertex()->GetName();
}

//----------------------------------------------------------------
G4String GmDataVertexLogicalVolume::GetStringValueFromSecoTrack( const G4Track* aTrack1, const G4Track* )
{
  return aTrack1->GetLogicalVolumeAtVertex()->GetName();

}


//----------------------------------------------------------------
G4String GmDataVertexLogicalVolume::GetStringValueFromEvent( const G4Event* anEvent )
{ 
  G4TouchableHistory* touch = new G4TouchableHistory;
  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndUpdateTouchable( anEvent->GetPrimaryVertex(0)->GetPosition(), touch, false ); 

  G4String name = touch->GetVolume()->GetLogicalVolume()->GetName();
 
  delete touch;

  return name;
}

//----------------------------------------------------------------
G4String GmDataVertexLogicalVolume::GetStringValueFromStackedTrack( const G4Track* aTrack )
{
  return GetPVFromPos(aTrack->GetPosition())->GetLogicalVolume()->GetName();
}
