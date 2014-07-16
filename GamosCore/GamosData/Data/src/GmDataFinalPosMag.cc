#include "GmDataFinalPosMag.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataFinalPosMag::GmDataFinalPosMag()
{
  bInitial = false;
  theHMin = 0.;
  theHMax = 1000.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalPosMag::~GmDataFinalPosMag()
{
}


//----------------------------------------------------------------
G4double GmDataFinalPosMag::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPostStepPoint()->GetPosition().mag();
}

//----------------------------------------------------------------
G4double GmDataFinalPosMag::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetPosition().mag();
}
