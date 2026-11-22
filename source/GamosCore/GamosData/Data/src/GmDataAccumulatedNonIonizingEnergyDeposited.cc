#include "GmDataAccumulatedNonIonizingEnergyDeposited.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataAccumulatedNonIonizingEnergyDeposited::GmDataAccumulatedNonIonizingEnergyDeposited()
{
  bInitial = false;
  theHMax = 1.;

  theExcludedTypes.clear(); // delete DTRun and DTEvent
  theExcludedTypes.insert(DTSeco);
}

//----------------------------------------------------------------
GmDataAccumulatedNonIonizingEnergyDeposited::~GmDataAccumulatedNonIonizingEnergyDeposited()
{
}

//----------------------------------------------------------------
void GmDataAccumulatedNonIonizingEnergyDeposited::Accumulate( const G4Step* aStep, G4int index )
{
  theDataAccumulated[index] += aStep->GetNonIonizingEnergyDeposit();
}

//----------------------------------------------------------------
G4double GmDataAccumulatedNonIonizingEnergyDeposited::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetNonIonizingEnergyDeposit();
}
