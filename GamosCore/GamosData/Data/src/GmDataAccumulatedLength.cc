#include "GmDataAccumulatedLength.hh"

#include "G4Step.hh"
#include "G4Track.hh"

//----------------------------------------------------------------
GmDataAccumulatedLength::GmDataAccumulatedLength()
{
  bInitial = false;
  theHMax = 10.;

  theExcludedTypes.clear(); // delete DTRun
  theExcludedTypes.insert(DTSeco);
}

//----------------------------------------------------------------
GmDataAccumulatedLength::~GmDataAccumulatedLength()
{
}

//----------------------------------------------------------------
void GmDataAccumulatedLength::Accumulate( const G4Step* aStep, G4int index )
{
 theDataAccumulated[index] += aStep->GetStepLength();
}

//----------------------------------------------------------------
G4double GmDataAccumulatedLength::GetValueFromStep( const G4Step* aStep, G4int )
{
  return aStep->GetStepLength();
}
