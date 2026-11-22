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
  G4Material* mate = GetMateFromPV( GetPVFromPos(aTrack->GetVertexPosition()) );
  return mate->GetRadlen();
}

//----------------------------------------------------------------
G4double GmDataInitialRadLength::GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track* , G4int )
{
  G4Material* mate = GetMateFromPV( GetPVFromPos(aTrack1->GetPosition()) );
  return mate->GetRadlen();
}

//----------------------------------------------------------------
G4double GmDataInitialRadLength::GetValueFromEvent( const G4Event* anEvent, G4int )
{
  G4Material* mate = GetMateFromPV( GetPVFromPos(anEvent->GetPrimaryVertex(0)->GetPosition()) );
  return mate->GetRadlen();
}

//----------------------------------------------------------------
G4double GmDataInitialRadLength::GetValueFromStackedTrack( const G4Track* aTrack, G4int )
{
  G4Material* mate = GetMateFromPV( GetPVFromPos(aTrack->GetPosition()) );
  return mate->GetRadlen();
}
