#include "G4TouchableHistory.hh"
#include "GmGeomConditionTraverseParallel.hh"
#include "GamosCore/GamosBase/Filters/include/GmFilterVerbosity.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosBase/Base/include/GmGetParticleMgr.hh"

//-------------------------------------------------------------------------
GmGeomConditionTraverseParallel::GmGeomConditionTraverseParallel()
{
   theTouchable = new G4TouchableHistory();
  
  std::vector<G4VProcess*> parallelWorldProcesses = GmGetParticleMgr::GetInstance()->GetG4ProcessList("ParaWorldProc_*");
  if( parallelWorldProcesses.size() == 0 ) {
    G4Exception("GmGeomConditionTraverseParallel::GmGeomConditionTraverseParallel",
		"No G4ParallelWorldScoringProcess named 'parallelWorldProcess_*' found",
		FatalErrorInArgument,
		"Check that you have not defined the filter before '/run/initialize'");
  }

  theParallelWorldProcess = (G4ParallelWorldScoringProcess*)(parallelWorldProcesses[0]);

}

//-------------------------------------------------------------------------
const G4VTouchable* GmGeomConditionTraverseParallel::GetTouchableFromTrack(const G4Track* aTrack)
{
#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionTraverseParallel::GetTouchableFromStep " << G4endl;
#endif
  // check it is in volume
  // check not exiting world
  if( aTrack->GetVolume() == 0 ) {
#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionTraverseParallel::GetTouchableFromStep return 0 " << G4endl;
#endif
    return 0;
  }

#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionTraverseParallel::GetTouchableFromStep return touchable ";
#endif
  if( aTrack->GetVolume() ) {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout  << aTrack->GetVolume()->GetName() << G4endl;
#endif
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << " 0 " << G4endl;
#endif
  }

  return aTrack->GetTouchable();
}

#include "G4TouchableHistory.hh"
#include "G4TransportationManager.hh"
#include "G4Navigator.hh"

//-------------------------------------------------------------------------
const G4VTouchable* GmGeomConditionTraverseParallel::GetTouchableFromStep(const G4Step* )
{
#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionTraverseParallel::GetTouchableFromStep " << G4endl;
#endif
  const G4StepPoint* preSP = theParallelWorldProcess->fGhostPreStepPoint;
  const G4StepPoint* postSP = theParallelWorldProcess->fGhostPostStepPoint;
  if( preSP->GetTouchable() != postSP->GetTouchable() ) {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionTraverseParallel::GetTouchableFromStep return 0  preSP " << preSP->GetTouchable() << " != " << postSP->GetTouchable() << G4endl;
#endif
    return 0;
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionTraverseParallel::GetTouchableFromStep return touchable ";
#endif
    if( preSP->GetPhysicalVolume() && preSP->GetPhysicalVolume()->GetName() ) {
#ifndef GAMOS_NO_VERBOSE
      if( FilterVerb(debugVerb) ) G4cout << preSP->GetPhysicalVolume()->GetName() << G4endl;
#endif
    } else {
#ifndef GAMOS_NO_VERBOSE
      if( FilterVerb(debugVerb) ) G4cout << " 0 " << G4endl;
#endif
    }
    return GetTouchableFromStepPoint( preSP );
  }

  //  G4cout << "GmGeomConditionTraverseParallel::GetTouchableFromStep parallel volume= " << preSP->GetTouchable()->GetVolume()->GetName() << " == " << preSP->GetPhysicalVolume()->GetName() << " mass volume= " << aStep->GetTrack()->GetVolume()->GetName() << G4endl;
}

