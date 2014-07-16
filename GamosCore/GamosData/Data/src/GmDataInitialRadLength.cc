#include "GmDataInitialRadLength.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4VPhysicalVolume.hh"
#include "G4TouchableHistory.hh"
#include "G4TransportationManager.hh"

//----------------------------------------------------------------
GmDataInitialRadLength::GmDataInitialRadLength()
{
  bInitial = true;
  theHMin = 0.;
  theHMax = 10.;
}

//----------------------------------------------------------------
GmDataInitialRadLength::~GmDataInitialRadLength()
{
}


//----------------------------------------------------------------
G4double GmDataInitialRadLength::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPreStepPoint()->GetMaterial()->GetRadlen();
}

//----------------------------------------------------------------
G4double GmDataInitialRadLength::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetLogicalVolumeAtVertex()->GetMaterial()->GetRadlen();
}

//----------------------------------------------------------------
G4double GmDataInitialRadLength::GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track* , G4int )
{
  return aTrack1->GetMaterial()->GetRadlen();
}

//----------------------------------------------------------------
G4double GmDataInitialRadLength::GetValueFromEvent( const G4Event* anEvent, G4int )
{
  G4TouchableHistory* touch = new G4TouchableHistory;
  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndUpdateTouchable( anEvent->GetPrimaryVertex(0)->GetPosition(), touch, false ); 

  G4double density = touch->GetVolume()->GetLogicalVolume()->GetMaterial()->GetRadlen();
 
  delete touch;

  return density;
}
