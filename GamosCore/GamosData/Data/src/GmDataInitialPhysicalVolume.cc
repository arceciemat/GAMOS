#include "GmDataInitialPhysicalVolume.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4VPhysicalVolume.hh"
#include "G4TouchableHistory.hh"
#include "G4TransportationManager.hh"

//----------------------------------------------------------------
GmDataInitialPhysicalVolume::GmDataInitialPhysicalVolume()
{
  bInitial = true;
  theNChar = 25;
  theNBytes = theNChar;

}

//----------------------------------------------------------------
GmDataInitialPhysicalVolume::~GmDataInitialPhysicalVolume()
{
}

//----------------------------------------------------------------
G4String GmDataInitialPhysicalVolume::GetStringValueFromStep( const G4Step* aStep )
{ 
  G4VPhysicalVolume* pv = aStep->GetPreStepPoint()->GetPhysicalVolume();
  return pv->GetName()+":"+GmGenUtils::ftoa(pv->GetCopyNo());
}

//----------------------------------------------------------------
G4String GmDataInitialPhysicalVolume::GetStringValueFromTrack( const G4Track* aTrack )
{ 
  G4TouchableHistory* touch = new G4TouchableHistory;
  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndUpdateTouchable( aTrack->GetVertexPosition(), touch, false ); 
  G4VPhysicalVolume* pv = touch->GetVolume();
  delete touch;

  G4String name = pv->GetName()+":"+GmGenUtils::ftoa(pv->GetCopyNo());

  return name;
}

//----------------------------------------------------------------
G4String GmDataInitialPhysicalVolume::GetStringValueFromSecoTrack( const G4Track* aTrack1, const G4Track* )
{
  return aTrack1->GetVolume()->GetLogicalVolume()->GetName();
}


//----------------------------------------------------------------
G4String GmDataInitialPhysicalVolume::GetStringValueFromEvent( const G4Event* anEvent )
{ 
  G4TouchableHistory* touch = new G4TouchableHistory;
  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndUpdateTouchable( anEvent->GetPrimaryVertex(0)->GetPosition(), touch, false ); 

  G4String name = touch->GetVolume()->GetName();
 
  delete touch;

  return name;
}
