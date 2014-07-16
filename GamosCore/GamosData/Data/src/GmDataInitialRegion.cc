#include "GmDataInitialRegion.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4TouchableHistory.hh"
#include "G4TransportationManager.hh"

//----------------------------------------------------------------
GmDataInitialRegion::GmDataInitialRegion()
{
  bInitial = true;
  theNChar = 25;
  theNBytes = theNChar;

  //  theExcludedTypes.insert(DTSeco);
}

//----------------------------------------------------------------
GmDataInitialRegion::~GmDataInitialRegion()
{
}

//----------------------------------------------------------------
G4String GmDataInitialRegion::GetStringValueFromStep( const G4Step* aStep )
{ 
  return aStep->GetPreStepPoint()->GetPhysicalVolume()->GetLogicalVolume()->GetRegion()->GetName();
}

//----------------------------------------------------------------
G4String GmDataInitialRegion::GetStringValueFromTrack( const G4Track* aTrack )
{ 
  return aTrack->GetLogicalVolumeAtVertex()->GetRegion()->GetName();
}

//----------------------------------------------------------------
G4String GmDataInitialRegion::GetStringValueFromSecoTrack( const G4Track* aTrack1, const G4Track* )
{
  return aTrack1->GetVolume()->GetLogicalVolume()->GetRegion()->GetName();
}


//----------------------------------------------------------------
G4String GmDataInitialRegion::GetStringValueFromEvent( const G4Event* anEvent )
{ 
  G4TouchableHistory* touch = new G4TouchableHistory;
  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndUpdateTouchable( anEvent->GetPrimaryVertex(0)->GetPosition(), touch, false ); 

  G4String name = touch->GetVolume()->GetLogicalVolume()->GetRegion()->GetName();
 
  delete touch;

  return name;
}
