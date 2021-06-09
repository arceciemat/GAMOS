#include "GmDataFinalElectronDensity.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmDataFinalElectronDensity::GmDataFinalElectronDensity()
{
  bInitial = false;
  theHMin = 0.;
  theHMax = 10.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalElectronDensity::~GmDataFinalElectronDensity()
{
}


//----------------------------------------------------------------
G4double GmDataFinalElectronDensity::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPostStepPoint()->GetMaterial()->GetElectronDensity()/CLHEP::g*CLHEP::cm3;
}

//----------------------------------------------------------------
G4double GmDataFinalElectronDensity::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetNextMaterial()->GetElectronDensity()/CLHEP::g*CLHEP::cm3;
}
