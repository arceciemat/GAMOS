#include "GmDataInitialElectronDensity.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4VPhysicalVolume.hh"
#include "G4TouchableHistory.hh"
#include "G4TransportationManager.hh"

//----------------------------------------------------------------
GmDataInitialElectronDensity::GmDataInitialElectronDensity()
{
  bInitial = true;
  theHMin = 0.;
  theHMax = 10.;
}

//----------------------------------------------------------------
GmDataInitialElectronDensity::~GmDataInitialElectronDensity()
{
}


//----------------------------------------------------------------
G4double GmDataInitialElectronDensity::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPreStepPoint()->GetMaterial()->GetElectronDensity()/CLHEP::g*CLHEP::cm3;
}

//----------------------------------------------------------------
G4double GmDataInitialElectronDensity::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  G4Material* mate = GetMateFromPV(  GetPVFromPos(aTrack->GetVertexPosition()) );
  return mate->GetElectronDensity()/CLHEP::g*CLHEP::cm3;
}

//----------------------------------------------------------------
G4double GmDataInitialElectronDensity::GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track* , G4int )
{
  return aTrack1->GetMaterial()->GetElectronDensity()/CLHEP::g*CLHEP::cm3;
}

//----------------------------------------------------------------
G4double GmDataInitialElectronDensity::GetValueFromEvent( const G4Event* anEvent, G4int )
{
  G4Material* mate = GetMateFromPV( GetPVFromPos(anEvent->GetPrimaryVertex(0)->GetPosition()) );
  return mate->GetElectronDensity()/CLHEP::g*CLHEP::cm3;
}


 //----------------------------------------------------------------
G4double GmDataInitialElectronDensity::GetValueFromStackedTrack( const G4Track* aTrack, G4int )
{
  G4Material* mate = GetMateFromPV( GetPVFromPos(aTrack->GetPosition()) );
  return mate->GetElectronDensity()/CLHEP::g*CLHEP::cm3;
}

