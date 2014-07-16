#include "GmDataFinalPVCopyNumber.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmDataFinalPVCopyNumber::GmDataFinalPVCopyNumber()
{
  bInitial = false;
  theExcludedTypes.clear(); // delete DTRun
}

//----------------------------------------------------------------
GmDataFinalPVCopyNumber::~GmDataFinalPVCopyNumber()
{
}


//----------------------------------------------------------------
G4double GmDataFinalPVCopyNumber::GetValueFromStep( const G4Step* aStep, G4int )
{

  return aStep->GetPreStepPoint()->GetPhysicalVolume()->GetCopyNo();

}

//----------------------------------------------------------------
G4double GmDataFinalPVCopyNumber::GetValueFromTrack( const G4Track* aTrack, G4int )
{

  return aTrack->GetVolume()->GetCopyNo();

}

//----------------------------------------------------------------
G4double GmDataFinalPVCopyNumber::GetValueFromSecoTrack(const G4Track* , const G4Track* aTrack2, G4int )
{

  return aTrack2->GetVolume()->GetCopyNo();
}
