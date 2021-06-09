#include "GmDataFinalNuclearInterLength.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmDataFinalNuclearInterLength::GmDataFinalNuclearInterLength()
{
  bInitial = false;
  theHMin = 0.;
  theHMax = 10.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalNuclearInterLength::~GmDataFinalNuclearInterLength()
{
}


//----------------------------------------------------------------
G4double GmDataFinalNuclearInterLength::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPostStepPoint()->GetMaterial()->GetNuclearInterLength();
}

//----------------------------------------------------------------
G4double GmDataFinalNuclearInterLength::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetNextMaterial()->GetNuclearInterLength();
}
