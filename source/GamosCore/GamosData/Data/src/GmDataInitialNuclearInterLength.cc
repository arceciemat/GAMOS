#include "GmDataInitialNuclearInterLength.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4VPhysicalVolume.hh"
#include "G4TouchableHistory.hh"
#include "G4TransportationManager.hh"

//----------------------------------------------------------------
GmDataInitialNuclearInterLength::GmDataInitialNuclearInterLength()
{
  bInitial = true;
  theHMin = 0.;
  theHMax = 10.;
}

//----------------------------------------------------------------
GmDataInitialNuclearInterLength::~GmDataInitialNuclearInterLength()
{
}


//----------------------------------------------------------------
G4double GmDataInitialNuclearInterLength::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPreStepPoint()->GetMaterial()->GetNuclearInterLength();
}

//----------------------------------------------------------------
G4double GmDataInitialNuclearInterLength::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  G4Material* mate = GetMateFromPV( GetPVFromPos(aTrack->GetVertexPosition()) );
  return mate->GetNuclearInterLength();
}

//----------------------------------------------------------------
G4double GmDataInitialNuclearInterLength::GetValueFromSecoTrack( const G4Track* aTrack1, const G4Track* , G4int )
{
  return aTrack1->GetMaterial()->GetNuclearInterLength();
}

//----------------------------------------------------------------
G4double GmDataInitialNuclearInterLength::GetValueFromEvent( const G4Event* anEvent, G4int )
{
  G4Material* mate = GetMateFromPV( GetPVFromPos(anEvent->GetPrimaryVertex(0)->GetPosition()) );
  return mate->GetNuclearInterLength();
}

//  LocalWords:  GmDataInitialNuclearInterLength

//----------------------------------------------------------------
G4double GmDataInitialNuclearInterLength::GetValueFromStackedTrack( const G4Track* aTrack, G4int )
{
  G4Material* mate = GetMateFromPV( GetPVFromPos(aTrack->GetPosition()) );
  return mate->GetNuclearInterLength();

}

