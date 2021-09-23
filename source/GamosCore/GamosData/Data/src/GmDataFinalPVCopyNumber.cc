#include "GmDataFinalPVCopyNumber.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"
#include "G4TouchableHistory.hh"
#include "G4TransportationManager.hh"

//----------------------------------------------------------------
GmDataFinalPVCopyNumber::GmDataFinalPVCopyNumber()
{
  bInitial = false;
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalPVCopyNumber::~GmDataFinalPVCopyNumber()
{
}


//----------------------------------------------------------------
G4double GmDataFinalPVCopyNumber::GetValueFromStep( const G4Step* aStep, G4int )
{

  return aStep->GetPostStepPoint()->GetTouchable()->GetReplicaNumber();

}

//----------------------------------------------------------------
G4double GmDataFinalPVCopyNumber::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetTouchable()->GetReplicaNumber();
}

//----------------------------------------------------------------
G4double GmDataFinalPVCopyNumber::GetValueFromSecoTrack(const G4Track* aTrack, const G4Track*, G4int )
{
  return aTrack->GetTouchable()->GetReplicaNumber();

}
