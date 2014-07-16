#include "GmDataFinalKineticEnergy.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataFinalKineticEnergy::GmDataFinalKineticEnergy()
{
  bInitial = false;
  theHMax = 1.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalKineticEnergy::~GmDataFinalKineticEnergy()
{
}


//----------------------------------------------------------------
G4double GmDataFinalKineticEnergy::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPostStepPoint()->GetKineticEnergy();
}

//----------------------------------------------------------------
G4double GmDataFinalKineticEnergy::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetKineticEnergy();
}
