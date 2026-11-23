#include "GmDataInitialDensity.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4VPhysicalVolume.hh"
#include "G4TouchableHistory.hh"
#include "G4TransportationManager.hh"
#include "G4PVParameterised.hh"

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
  G4Material* mate = GetMateFromPV( GetPVFromPos(aTrack->GetVertexPosition()) );
  return mate->GetDensity()/CLHEP::g*CLHEP::cm3;
}

//----------------------------------------------------------------
G4double GmDataInitialDensity::GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track* , G4int )
{
  G4Material* mate = GetMateFromPV( GetPVFromPos(aTrack1->GetPosition()) );
  return mate->GetDensity()/CLHEP::g*CLHEP::cm3;
}

//----------------------------------------------------------------
G4double GmDataInitialDensity::GetValueFromEvent( const G4Event* anEvent, G4int )
{
  G4Material* mate = GetMateFromPV( GetPVFromPos(anEvent->GetPrimaryVertex(0)->GetPosition()) );
  return mate->GetDensity()/CLHEP::g*CLHEP::cm3;

}


 //----------------------------------------------------------------
G4double GmDataInitialDensity::GetValueFromStackedTrack( const G4Track* aTrack, G4int )
{
  G4Material* mate = GetMateFromPV( GetPVFromPos(aTrack->GetPosition()) );
  return mate->GetDensity()/CLHEP::g*CLHEP::cm3;
}

