#include "GmDataInitialPhysicalVolume.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosData/Management/include/GmDataVerbosity.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VTouchable.hh"
#include "G4Event.hh"
#include "G4TransportationManager.hh"
#include "G4TouchableHistory.hh"
#include "G4Navigator.hh"

//----------------------------------------------------------------
GmDataInitialPhysicalVolume::GmDataInitialPhysicalVolume()
{
  bInitial = false;
  theNChar = 25;
  theNBytes = theNChar;

  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataInitialPhysicalVolume::~GmDataInitialPhysicalVolume()
{
}

//----------------------------------------------------------------
G4String GmDataInitialPhysicalVolume::GetStringValueFromStep( const G4Step* aStep )
{ 
  return  GmG4Utils::GetNameNumberFromTouchable(aStep->GetPreStepPoint()->GetTouchable());

}

//----------------------------------------------------------------
G4String GmDataInitialPhysicalVolume::GetStringValueFromTrack( const G4Track* aTrack )
{ 
  return  GmG4Utils::GetNameNumberFromTouchable(aTrack->GetTouchable());
}


//----------------------------------------------------------------
G4String GmDataInitialPhysicalVolume::GetStringValueFromSecoTrack( const G4Track* aTrack1, const G4Track* )
{

  G4TouchableHistory* touch = new G4TouchableHistory;
  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndUpdateTouchable( aTrack1->GetVertexPosition(), touch, false ); 
  return GmG4Utils::GetNameNumberFromTouchable(touch);
}


//----------------------------------------------------------------
G4String GmDataInitialPhysicalVolume::GetStringValueFromEvent( const G4Event* anEvent )
{ 
  G4TouchableHistory* touch = new G4TouchableHistory;
  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndUpdateTouchable( anEvent->GetPrimaryVertex(0)->GetPosition(), touch, false ); 

   return GmG4Utils::GetNameNumberFromTouchable(touch);
}

//----------------------------------------------------------------
G4String GmDataInitialPhysicalVolume::GetStringValueFromStackedTrack( const G4Track* aTrack )
{ 
  
  G4TouchableHistory* touch = new G4TouchableHistory;
  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndUpdateTouchable( aTrack->GetPosition(), touch, false );
  
  return GmG4Utils::GetNameNumberFromTouchable(touch);
}
