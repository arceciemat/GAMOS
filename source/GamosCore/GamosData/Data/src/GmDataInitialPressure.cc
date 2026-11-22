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
  G4Material* mate = GetMateFromPV( GetPVFromPos(aTrack->GetVertexPosition()) );
  return mate->GetPressure()/CLHEP::atmosphere;
}

//----------------------------------------------------------------
G4double GmDataInitialPressure::GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track* , G4int )
{
  G4Material* mate = GetMateFromPV( GetPVFromPos(aTrack1->GetPosition()) );
  return mate->GetPressure()/CLHEP::atmosphere;
}

//----------------------------------------------------------------
G4double GmDataInitialPressure::GetValueFromEvent( const G4Event* anEvent, G4int )
{
  G4Material* mate = GetMateFromPV( GetPVFromPos(anEvent->GetPrimaryVertex(0)->GetPosition()) );
  return mate->GetPressure()/CLHEP::atmosphere;

}

//----------------------------------------------------------------
G4double GmDataInitialPressure::GetValueFromStackedTrack( const G4Track* aTrack, G4int )
{
  G4Material* mate = GetMateFromPV( GetPVFromPos(aTrack->GetPosition()) );
  return mate->GetPressure()/CLHEP::atmosphere;
}
