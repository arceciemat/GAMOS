#include "G4TouchableHistory.hh"
#include "GmGeomConditionStartParallel.hh"
#include "GamosCore/GamosBase/Filters/include/GmFilterVerbosity.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosBase/Base/include/GmGetParticleMgr.hh"

//-------------------------------------------------------------------------
GmGeomConditionStartParallel::GmGeomConditionStartParallel()
{
	
  theTouchable = new G4TouchableHistory();
  
  std::vector<G4VProcess*> parallelWorldProcesses = GmGetParticleMgr::GetInstance()->GetG4ProcessList("ParaWorldProc_*");
  if( parallelWorldProcesses.size() == 0 ) {
    G4Exception("GmGeomConditionStartParallel::GmGeomConditionStartParallel",
		"No G4ParallelWorldScoringProcess named 'parallelWorldProcess_*' found",
		FatalErrorInArgument,
		"Check that you have not defined the filter before '/run/initialize'");
  }

  theParallelWorldProcess = (G4ParallelWorldScoringProcess*)(parallelWorldProcesses[0]);

}

//-------------------------------------------------------------------------
const G4VTouchable* GmGeomConditionStartParallel::GetTouchableFromTrack(const G4Track* aTrack)
{

#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionStartParallel::GetTouchableFromTrack step number " << aTrack->GetCurrentStepNumber() << G4endl;
#endif
  if( aTrack->GetCurrentStepNumber() != 0 ) {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionStartParallel::GetTouchableFromTrack return 0 " << aTrack->GetCurrentStepNumber() << G4endl;
#endif
    return 0;
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionStartParallel::GetTouchableFromTrack return touchable " << aTrack->GetVolume()->GetName() << G4endl;
#endif
    return aTrack->GetTouchable();
  }
  
}

#include "G4TouchableHistory.hh"
#include "G4TransportationManager.hh"
#include "G4Navigator.hh"

//-------------------------------------------------------------------------
const G4VTouchable* GmGeomConditionStartParallel::GetTouchableFromStep(const G4Step* aStep)
{
	
#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionStartParallel::GetTouchableFromStep step number " << aStep->GetTrack()->GetCurrentStepNumber() << G4endl;
#endif
  if( aStep->GetTrack()->GetCurrentStepNumber() != 1 ) {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionStartParallel::GetTouchableFromStep return 0 " << G4endl;
#endif
    return 0;
  } else {
    const G4StepPoint* preSP = theParallelWorldProcess->fGhostPreStepPoint;
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionStartParallel::GetTouchableFromStep return ";
#endif
    if( preSP->GetTouchable() && preSP->GetTouchable()->GetVolume() ) {
#ifndef GAMOS_NO_VERBOSE
      if( FilterVerb(debugVerb) ) G4cout << preSP->GetTouchable()->GetVolume()->GetName() << G4endl; 
#endif
    } else {
#ifndef GAMOS_NO_VERBOSE
      if( FilterVerb(debugVerb) ) G4cout << " 0 " << G4endl;
#endif
    }
    return GetTouchableFromStepPoint( preSP );
  }
//  G4cout << "GmGeomConditionStartParallel::GetTouchableFromStep parallel volume= " << preSP->GetTouchable()->GetVolume()->GetName() << " == " << preSP->GetPhysicalVolume()->GetName() << " mass volume= " << aStep->GetTrack()->GetVolume()->GetName() << G4endl;
}

