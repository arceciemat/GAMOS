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
  G4Material* mate = aStep->GetPostStepPoint()->GetMaterial();
  if( mate ) {
    return mate->GetRadlen();
  } else {
    return 0.;
  }
}

//----------------------------------------------------------------
G4double GmDataFinalRadLength::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  G4Material* mate = aTrack->GetNextMaterial();
  if( mate ) {
    return mate->GetRadlen();
  } else {
    return 0.;
  }
}
