#include "G4TouchableHistory.hh"
#include "GmGeomConditionEnterParallel.hh"
#include "GamosCore/GamosBase/Filters/include/GmFilterVerbosity.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosBase/Base/include/GmGetParticleMgr.hh"

//-------------------------------------------------------------------------
GmGeomConditionEnterParallel::GmGeomConditionEnterParallel()
{
  theTouchable = new G4TouchableHistory();
  
  std::vector<G4VProcess*> parallelWorldProcesses = GmGetParticleMgr::GetInstance()->GetG4ProcessList("ParaWorldProc_*");
  if( parallelWorldProcesses.size() == 0 ) {
    G4Exception("GmGeomConditionEnterParallel::GmGeomConditionEnterParallel",
		"No G4ParallelWorldScoringProcess named 'parallelWorldProcess_*' found",
		FatalErrorInArgument,
		"Check that you have not defined the filter before '/run/initialize'");
  }

  theParallelWorldProcess = (G4ParallelWorldScoringProcess*)(parallelWorldProcesses[0]);

}

//-------------------------------------------------------------------------
const G4VTouchable* GmGeomConditionEnterParallel::GetTouchableFromTrack(const G4Track* )
{
#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionEnterParallel::GetTouchableFromTrack return 0 " << G4endl;
#endif
  return 0;
}

#include "G4TouchableHistory.hh"
#include "G4TransportationManager.hh"
#include "G4Navigator.hh"

//-------------------------------------------------------------------------
const G4VTouchable* GmGeomConditionEnterParallel::GetTouchableFromStep(const G4Step* aStep)
{
#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionEnterParallel::GetTouchableFromStep " << G4endl;
#endif

  const G4StepPoint* postSP = theParallelWorldProcess->fGhostPostStepPoint;
  if( postSP->GetStepStatus() != fGeomBoundary ) {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionEnterParallel::GetTouchableFromStep return 0, stepStatus " << postSP->GetStepStatus() << G4endl;
#endif
    return 0;
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << "GmGeomConditionEnterParallel::GetTouchableFromStep return touchable ";
#endif
    if( postSP->GetTouchable() && postSP->GetTouchable()->GetVolume() ) {
#ifndef GAMOS_NO_VERBOSE
      if( FilterVerb(debugVerb) ) G4cout 
	     << postSP->GetTouchable()->GetVolume()->GetName() << " " << aStep->GetTrack()->GetVolume()->GetName() << G4endl;
#endif
    } else {
#ifndef GAMOS_NO_VERBOSE
      if( FilterVerb(debugVerb) ) G4cout << " 0 " << G4endl;
#endif
    }
    return GetTouchableFromStepPoint( postSP );
  }

  //  G4cout << "GmGeomConditionEnterParallel::GetTouchableFromStep parallel volume= " << postSP->GetTouchable()->GetVolume()->GetName() << " == " << postSP->GetPhysicalVolume()->GetName() << " mass volume= " << aStep->GetTrack()->GetVolume()->GetName() << G4endl;
}
