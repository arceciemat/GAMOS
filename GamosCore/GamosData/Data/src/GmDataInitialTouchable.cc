#include "GmDataInitialTouchable.hh"

#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4LogicalVolume.hh"
#include "G4TransportationManager.hh"
#include "G4TouchableHistory.hh"
#include "G4Navigator.hh"

//----------------------------------------------------------------
GmDataInitialTouchable::GmDataInitialTouchable()
{
  bInitial = true;
  theNChar = 25;
  theNBytes = theNChar;

  //  theExcludedTypes.insert(DTSeco);
}

//----------------------------------------------------------------
GmDataInitialTouchable::~GmDataInitialTouchable()
{
}

//----------------------------------------------------------------
G4String GmDataInitialTouchable::GetStringValueFromStep( const G4Step* aStep )
{ 
  return GetTouchableLongName( aStep->GetPreStepPoint()->GetTouchable() );
}

//----------------------------------------------------------------
G4String GmDataInitialTouchable::GetStringValueFromTrack( const G4Track* aTrack )
{ 
  G4TouchableHistory* touch = new G4TouchableHistory;
  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndUpdateTouchable( aTrack->GetVertexPosition(), touch, false ); 

  G4String name = GetTouchableLongName( touch );
 
  delete touch;

  return name;
}

//----------------------------------------------------------------
G4String GmDataInitialTouchable::GetStringValueFromSecoTrack( const G4Track* aTrack1, const G4Track* )
{
  return GetTouchableLongName( aTrack1->GetNextTouchable() );
}

//----------------------------------------------------------------
G4String GmDataInitialTouchable::GetStringValueFromEvent( const G4Event* anEvent )
{ 
  G4TouchableHistory* touch = new G4TouchableHistory;
  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndUpdateTouchable( anEvent->GetPrimaryVertex(0)->GetPosition(), touch, false ); 

  G4String name = GetTouchableLongName( touch );
 
  delete touch;

  return name;
}

//----------------------------------------------------------------
G4String GmDataInitialTouchable::GetTouchableLongName( const G4VTouchable* touch )
{
  G4int nAnces = touch->GetHistoryDepth();
  G4String name;
  for( G4int ii = nAnces; ii >= 0; ii-- ) {
    name += "/" + touch->GetVolume(ii)->GetName();
    name += ":" + GmGenUtils::itoa( touch->GetCopyNumber(ii) );
  }

  return name;
}
