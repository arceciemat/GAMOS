#include "GmDataTimeChange.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataTimeChange::GmDataTimeChange()
{
  theHMax = 1.E9;
  theExcludedTypes.insert(DTEvent);
  theExcludedTypes.insert(DTTrack);
  theExcludedTypes.insert(DTSeco);
}

//----------------------------------------------------------------
GmDataTimeChange::~GmDataTimeChange()
{
}

//----------------------------------------------------------------
G4double GmDataTimeChange::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPostStepPoint()->GetGlobalTime() - aStep->GetPreStepPoint()->GetGlobalTime();
}
