#include "GmDataInitialSolidType.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4LogicalVolume.hh"
#include "G4VSolid.hh"
#include "G4VPhysicalVolume.hh"
#include "G4TouchableHistory.hh"
#include "G4TransportationManager.hh"

//----------------------------------------------------------------
GmDataInitialSolidType::GmDataInitialSolidType()
{
  bInitial = true;
  theNChar = 25;
  theNBytes = theNChar;

  //  theExcludedTypes.insert(DTSeco);
}

//----------------------------------------------------------------
GmDataInitialSolidType::~GmDataInitialSolidType()
{
}

//----------------------------------------------------------------
G4String GmDataInitialSolidType::GetStringValueFromStep( const G4Step* aStep )
{ 
  return aStep->GetPreStepPoint()->GetPhysicalVolume()->GetLogicalVolume()->GetSolid()->GetEntityType();
}

//----------------------------------------------------------------
G4String GmDataInitialSolidType::GetStringValueFromTrack( const G4Track* aTrack )
{ 
  return aTrack->GetLogicalVolumeAtVertex()->GetSolid()->GetEntityType();
}

//----------------------------------------------------------------
G4String GmDataInitialSolidType::GetStringValueFromSecoTrack( const G4Track* aTrack1, const G4Track* )
{
  return aTrack1->GetVolume()->GetLogicalVolume()->GetSolid()->GetEntityType();
}


//----------------------------------------------------------------
G4String GmDataInitialSolidType::GetStringValueFromEvent( const G4Event* anEvent )
{ 
  G4TouchableHistory* touch = new G4TouchableHistory;
 G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndUpdateTouchable( anEvent->GetPrimaryVertex(0)->GetPosition(), touch, false ); 

  G4String name = touch->GetVolume()->GetLogicalVolume()->GetSolid()->GetEntityType();
 
  delete touch;

  return name;
}
