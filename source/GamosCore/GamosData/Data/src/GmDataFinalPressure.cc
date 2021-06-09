#include "GmDataFinalPressure.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmDataFinalPressure::GmDataFinalPressure()
{
  bInitial = false;
  theHMin = 0.;
  theHMax = 10.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalPressure::~GmDataFinalPressure()
{
}


//----------------------------------------------------------------
G4double GmDataFinalPressure::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPostStepPoint()->GetMaterial()->GetPressure()/CLHEP::atmosphere;
}

//----------------------------------------------------------------
G4double GmDataFinalPressure::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetNextMaterial()->GetPressure()/CLHEP::atmosphere;
}
