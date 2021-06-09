#include "GmDataFinalParallelPVCopyNumber.hh"

#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosBase/Base/include/GmGetParticleMgr.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Navigator.hh"
#include "G4VPhysicalVolume.hh"
#include "G4TouchableHistory.hh"

//----------------------------------------------------------------
GmDataFinalParallelPVCopyNumber::GmDataFinalParallelPVCopyNumber()
{
  bInitial = false;
  theExcludedTypes.insert(DTSeco);
  theExcludedTypes.insert(DTEvent);

  std::vector<G4VProcess*> parallelWorldProcesses = GmGetParticleMgr::GetInstance()->GetG4ProcessList("parallelWorldProcess_*");
  if( parallelWorldProcesses.size() == 0 ) {
    G4Exception("GmGeomConditionStartParallel::GmGeomConditionStartParallel",
		"No G4ParallelWorldScoringProcess named 'parallelWorldProcess_*' found",
		FatalErrorInArgument,
		"Check that you have not defined the filter before '/run/initialize'");
  }

  theParallelWorldProcess = (G4ParallelWorldScoringProcess*)(parallelWorldProcesses[0]);
  
}

//----------------------------------------------------------------
GmDataFinalParallelPVCopyNumber::~GmDataFinalParallelPVCopyNumber()
{
}


//----------------------------------------------------------------
G4double GmDataFinalParallelPVCopyNumber::GetValueFromStep( const G4Step* , G4int )
{
  const G4StepPoint* postSP = theParallelWorldProcess->fGhostPreStepPoint;
  return postSP->GetPhysicalVolume()->GetCopyNo();
}

//----------------------------------------------------------------
G4double GmDataFinalParallelPVCopyNumber::GetValueFromTrack( const G4Track* aTrack, G4int )
{
 /* G4TouchableHistory* touch = new G4TouchableHistory;
  theParallelWorldProcess->fGhostNavigator->LocateGlobalPointAndUpdateTouchable( aTrack->GetPosition(), touch, false ); 
  
  G4int copyNo = touch->GetVolume()->GetCopyNo();
  
  delete touch;

  return copyNo; */
	return 0;
}

