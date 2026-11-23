#include "GmDataAccumulatedNonIonizingEnergyDeposit.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataAccumulatedNonIonizingEnergyDeposit::GmDataAccumulatedNonIonizingEnergyDeposit()
{
  bInitial = false;
  theHMax = 1.;

  theExcludedTypes.clear(); // delete DTRun and DTEvent
  theExcludedTypes.insert(DTSeco);
}

//----------------------------------------------------------------
GmDataAccumulatedNonIonizingEnergyDeposit::~GmDataAccumulatedNonIonizingEnergyDeposit()
{
}

//----------------------------------------------------------------
void GmDataAccumulatedNonIonizingEnergyDeposit::Accumulate( const G4Step* aStep, G4int index )
{
  theDataAccumulated[index] += aStep->GetNonIonizingEnergyDeposit();
}

//----------------------------------------------------------------
G4double GmDataAccumulatedNonIonizingEnergyDeposit::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetNonIonizingEnergyDeposit();
}
