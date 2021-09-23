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
  G4Material* mate = aStep->GetPostStepPoint()->GetMaterial();
  if( mate ) {
    return mate->GetElectronDensity()/CLHEP::g*CLHEP::cm3;
  } else {
    return 0.;
  } 
}

//----------------------------------------------------------------
G4double GmDataFinalElectronDensity::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  G4Material* mate = aTrack->GetNextMaterial();
  if( mate ) {
    return mate->GetElectronDensity()/CLHEP::g*CLHEP::cm3;
  } else {
    return 0.;
  } 
}
