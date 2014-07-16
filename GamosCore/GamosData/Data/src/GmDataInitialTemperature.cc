#include "GmDataInitialTemperature.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4VPhysicalVolume.hh"
#include "G4TouchableHistory.hh"
#include "G4TransportationManager.hh"

//----------------------------------------------------------------
GmDataInitialTemperature::GmDataInitialTemperature()
{
  bInitial = true;
  theHMin = 0.;
  theHMax = 1000.;
}

//----------------------------------------------------------------
GmDataInitialTemperature::~GmDataInitialTemperature()
{
}


//----------------------------------------------------------------
G4double GmDataInitialTemperature::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPreStepPoint()->GetMaterial()->GetTemperature();
}

//----------------------------------------------------------------
G4double GmDataInitialTemperature::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetLogicalVolumeAtVertex()->GetMaterial()->GetTemperature();
}

//----------------------------------------------------------------
G4double GmDataInitialTemperature::GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track* , G4int )
{
  return aTrack1->GetMaterial()->GetTemperature();
}

//----------------------------------------------------------------
G4double GmDataInitialTemperature::GetValueFromEvent( const G4Event* anEvent, G4int )
{
  G4TouchableHistory* touch = new G4TouchableHistory;
  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndUpdateTouchable( anEvent->GetPrimaryVertex(0)->GetPosition(), touch, false ); 

  G4double density = touch->GetVolume()->GetLogicalVolume()->GetMaterial()->GetTemperature();
 
  delete touch;

  return density;
}
