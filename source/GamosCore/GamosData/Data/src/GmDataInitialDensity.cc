#include "GmDataInitialDensity.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4VPhysicalVolume.hh"
#include "G4TouchableHistory.hh"
#include "G4TransportationManager.hh"

//----------------------------------------------------------------
GmDataInitialDensity::GmDataInitialDensity()
{
  bInitial = true;
  theHMin = 0.;
  theHMax = 10.;
}

//----------------------------------------------------------------
GmDataInitialDensity::~GmDataInitialDensity()
{
}


//----------------------------------------------------------------
G4double GmDataInitialDensity::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPreStepPoint()->GetMaterial()->GetDensity()/CLHEP::g*CLHEP::cm3;
}

//----------------------------------------------------------------
G4double GmDataInitialDensity::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetLogicalVolumeAtVertex()->GetMaterial()->GetDensity()/CLHEP::g*CLHEP::cm3;
}

//----------------------------------------------------------------
G4double GmDataInitialDensity::GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track* , G4int )
{
  return aTrack1->GetMaterial()->GetDensity()/CLHEP::g*CLHEP::cm3;
}

//----------------------------------------------------------------
G4double GmDataInitialDensity::GetValueFromEvent( const G4Event* anEvent, G4int )
{
  G4TouchableHistory* touch = new G4TouchableHistory;
  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndUpdateTouchable( anEvent->GetPrimaryVertex(0)->GetPosition(), touch, false ); 

  G4double density = touch->GetVolume()->GetLogicalVolume()->GetMaterial()->GetDensity()/CLHEP::g*CLHEP::cm3;
 
  delete touch;

  return density;
}
