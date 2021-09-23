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
  G4Material* mate = GetMateFromPV( GetPVFromPos(aTrack->GetVertexPosition()) );
  return mate->GetTemperature();
}

//----------------------------------------------------------------
G4double GmDataInitialTemperature::GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track* , G4int )
{
  G4Material* mate = GetMateFromPV( GetPVFromPos(aTrack1->GetPosition()) );
  return mate->GetTemperature();
}

//----------------------------------------------------------------
G4double GmDataInitialTemperature::GetValueFromEvent( const G4Event* anEvent, G4int )
{
  G4Material* mate = GetMateFromPV( GetPVFromPos(anEvent->GetPrimaryVertex(0)->GetPosition()) );
  return mate->GetTemperature();
}

//----------------------------------------------------------------
G4double GmDataInitialTemperature::GetValueFromStackedTrack( const G4Track* aTrack, G4int)
{ 
  G4Material* mate = GetMateFromPV( GetPVFromPos(aTrack->GetPosition()) );
  return mate->GetTemperature();
}

