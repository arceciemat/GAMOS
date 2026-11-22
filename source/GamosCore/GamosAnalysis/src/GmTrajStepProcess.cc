#include "GmTrajStepProcess.hh"
#include "G4Step.hh"
#include "G4VProcess.hh"
#include "G4TrackingManager.hh"
#include "G4SteppingManager.hh"
#include "G4TrackVector.hh"


//----------------------------------------------------------------------
GmTrajStepProcess::GmTrajStepProcess( const G4Step* aStep, TSPUse)
{
  theProcess = aStep->GetPostStepPoint()->GetProcessDefinedStep();
  theProcessName = theProcess->GetProcessName();
}
