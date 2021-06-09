#include "GmDataInitialPressure.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4VPhysicalVolume.hh"
#include "G4TouchableHistory.hh"
#include "G4TransportationManager.hh"

//----------------------------------------------------------------
GmDataInitialPressure::GmDataInitialPressure()
{
  bInitial = true;
  theHMin = 0.;
  theHMax = 10.;
}

//----------------------------------------------------------------
GmDataInitialPressure::~GmDataInitialPressure()
{
}


//----------------------------------------------------------------
G4double GmDataInitialPressure::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPreStepPoint()->GetMaterial()->GetPressure()/CLHEP::atmosphere;
}

//----------------------------------------------------------------
G4double GmDataInitialPressure::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetLogicalVolumeAtVertex()->GetMaterial()->GetPressure()/CLHEP::atmosphere;
}

//----------------------------------------------------------------
G4double GmDataInitialPressure::GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track* , G4int )
{
  return aTrack1->GetMaterial()->GetPressure()/CLHEP::atmosphere;
}

//----------------------------------------------------------------
G4double GmDataInitialPressure::GetValueFromEvent( const G4Event* anEvent, G4int )
{
  G4TouchableHistory* touch = new G4TouchableHistory;
  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndUpdateTouchable( anEvent->GetPrimaryVertex(0)->GetPosition(), touch, false ); 

  G4double density = touch->GetVolume()->GetLogicalVolume()->GetMaterial()->GetPressure()/CLHEP::atmosphere;
 
  delete touch;

  return density;
}
