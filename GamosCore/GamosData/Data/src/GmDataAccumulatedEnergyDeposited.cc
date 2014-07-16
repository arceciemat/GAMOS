#include "GmDataAccumulatedEnergyDeposited.hh"
#include "GamosCore/GamosData/Management/include/GmDataVerbosity.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataAccumulatedEnergyDeposited::GmDataAccumulatedEnergyDeposited()
{
  bInitial = false;
  theHMax = 1.;

  theExcludedTypes.clear(); // delete DTRun
  theExcludedTypes.insert(DTSeco);
}

//----------------------------------------------------------------
GmDataAccumulatedEnergyDeposited::~GmDataAccumulatedEnergyDeposited()
{
}

//----------------------------------------------------------------
void GmDataAccumulatedEnergyDeposited::Accumulate( const G4Step* aStep, G4int index )
{
  theDataAccumulated[index] += aStep->GetTotalEnergyDeposit();
#ifndef GAMOS_NO_VERBOSE
  if( DataVerb(debugVerb) ) G4cout << " GmDataAccumulatedEnergyDeposited::Accumulate " << index << " = " << theDataAccumulated[index] << " energyDepo " <<  aStep->GetTotalEnergyDeposit() << G4endl;
#endif
}

//----------------------------------------------------------------
G4double GmDataAccumulatedEnergyDeposited::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetTotalEnergyDeposit();
}
