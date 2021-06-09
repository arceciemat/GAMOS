#include "GmDataFinalDensity.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmDataFinalDensity::GmDataFinalDensity()
{
  bInitial = false;
  theHMin = 0.;
  theHMax = 10.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalDensity::~GmDataFinalDensity()
{
}


//----------------------------------------------------------------
G4double GmDataFinalDensity::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPostStepPoint()->GetMaterial()->GetDensity()/CLHEP::g*CLHEP::cm3;
}

//----------------------------------------------------------------
G4double GmDataFinalDensity::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetNextMaterial()->GetDensity()/CLHEP::g*CLHEP::cm3;
}
