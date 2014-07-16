#include "GmDataFinalTotalEnergy.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataFinalTotalEnergy::GmDataFinalTotalEnergy()
{
  bInitial = false;
  theHMax = 1.;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);
}

//----------------------------------------------------------------
GmDataFinalTotalEnergy::~GmDataFinalTotalEnergy()
{
}


//----------------------------------------------------------------
G4double GmDataFinalTotalEnergy::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetPostStepPoint()->GetKineticEnergy()+aStep->GetTrack()->GetDefinition()->GetPDGMass();
}

//----------------------------------------------------------------
G4double GmDataFinalTotalEnergy::GetValueFromTrack( const G4Track* aTrack, G4int )
{
  return aTrack->GetKineticEnergy()+aTrack->GetDefinition()->GetPDGMass();
}
