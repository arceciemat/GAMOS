#include "GmTrajStepEner.hh"
#include "G4Step.hh"
#include "G4VProcess.hh"
#include "G4TrackingManager.hh"
#include "G4SteppingManager.hh"
#include "G4TrackVector.hh"


//----------------------------------------------------------------------
GmTrajStepEner::GmTrajStepEner( const G4Step* aStep, TSPUse pointToUse )
{
  thePointToUse = pointToUse;

  if( pointToUse == TSPUPrePost || pointToUse == TSPUPre ) {
    G4StepPoint* sp = aStep->GetPreStepPoint();
    theKineticEnergyPre = sp->GetKineticEnergy();
  }

  if( pointToUse == TSPUPrePost || pointToUse == TSPUPost ) {
    G4StepPoint* sp = aStep->GetPostStepPoint();
    theKineticEnergyPost = sp->GetKineticEnergy();
  }

  theDepositedEnergy = aStep->GetTotalEnergyDeposit();
}

//----------------------------------------------------------------------
G4double GmTrajStepEner::GetDeltaKineticEnergy() const 
{ 
  if( thePointToUse == TSPUPrePost ) {
    return theKineticEnergyPre-theKineticEnergyPost;
  } else {
    G4Exception("GmTrajStepPosMom::GetKineticEnergy",
		"Error",
		FatalException,
		"Asking when not Pre and Post variables are filled, it is not possible to know which to use. Please instantiate it with TSPUPrePost");
  }

  return 0.;
}
