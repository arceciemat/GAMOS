#include "GmTrajStep.hh"

//----------------------------------------------------------------------
GmTrajStep::GmTrajStep( const G4Step* aStep, TSPUse pointToUse ) : GmTrajStepPosMom(aStep, pointToUse), GmTrajStepEner(aStep, pointToUse), GmTrajStepProcess(aStep, pointToUse)
{
}


//----------------------------------------------------------------------
GmTrajStep::~GmTrajStep()
{
}
