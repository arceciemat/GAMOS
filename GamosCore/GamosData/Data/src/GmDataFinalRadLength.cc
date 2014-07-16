#include "GmDataFinalRadLength.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmDataFinalRadLength::GmDataFinalRadLength()
{
  bInitial = false;
  theHMin = 0.;
  theHMax = 10.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalRadLength::~GmDataFinalRadLength()
{
}


//----------------------------------------------------------------
G4double GmDataFinalRadLength::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPostStepPoint()->GetMaterial()->GetRadlen();
}

//----------------------------------------------------------------
G4double GmDataFinalRadLength::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetNextMaterial()->GetRadlen();
}
