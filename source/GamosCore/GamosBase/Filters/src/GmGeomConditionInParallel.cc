#include "G4TouchableHistory.hh"
#include "GmGeomConditionInParallel.hh"
#include "GamosCore/GamosBase/Filters/include/GmFilterVerbosity.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "GamosCore/GamosUtils/include/GmG4Utils.hh"
#include "GamosCore/GamosBase/Base/include/GmGetParticleMgr.hh"

//-------------------------------------------------------------------------
GmGeomConditionInParallel::GmGeomConditionInParallel()
{
  theTouchable = new G4TouchableHistory();
  
  std::vector<G4VProcess*> parallelWorldProcesses = GmGetParticleMgr::GetInstance()->GetG4ProcessList("ParaWorldProc*");
  if( parallelWorldProcesses.size() == 0 ) {
    G4Exception("GmGeomConditionInParallel::GmGeomConditionInParallel",
		"No G4ParallelWorldScoringProcess named 'ParaWorldProc*' found",
		FatalErrorInArgument,
		"Check that you have not defined the filter before '/run/initialize'");
  }

  G4cout << " NUMBER parallelWorldProcesses " << parallelWorldProcesses.size() << G4endl; //GDEB
  theParallelWorldProcess = (G4ParallelWorldScoringProcess*)(parallelWorldProcesses[0]);

}

//-------------------------------------------------------------------------
const G4VTouchable* GmGeomConditionInParallel::GetTouchableFromTrack(const G4Track* aTrack)
{
  //--- No filtering for G4Track's
#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionInParallel::GetTouchableFromTrack " << G4endl;
#endif
  // check not exiting world
  if( aTrack->GetVolume() == 0 ) {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionInParallel::GetTouchableFromTrack return 0 " << G4endl;
#endif
    return 0;
  }

#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionInParallel::GetTouchableFromTrack return touchable " << aTrack->GetTouchable()->GetVolume()->GetName() << " " << aTrack->GetVolume()->GetName() << G4endl;
#endif
  return aTrack->GetTouchable();

}

#include "G4TouchableHistory.hh"
#include "G4TransportationManager.hh"
#include "G4Navigator.hh"

//-------------------------------------------------------------------------
const G4VTouchable* GmGeomConditionInParallel::GetTouchableFromStep(const G4Step* aStep)
{
#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionInParallel::GetTouchableFromStep " << G4endl;
#endif

  const G4StepPoint* preSP = theParallelWorldProcess->fGhostPreStepPoint;
#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << "GmGeomConditionInParallel::GetTouchableFromStep return touchable ";
#endif
  if( preSP->GetTouchable() && preSP->GetTouchable()->GetVolume() ) {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << preSP->GetTouchable()->GetVolume()->GetName() << " " << aStep->GetTrack()->GetVolume()->GetName() << G4endl;
#endif
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << G4endl;
#endif
  }

    //    G4cout << "GmGeomConditionInParallel::GetTouchableFromStep parallel volume= " << preSP->GetTouchable()->GetVolume()->GetName() << " == " << preSP->GetPhysicalVolume()->GetName() << " mass volume= " << aStep->GetTrack()->GetVolume()->GetName() << G4endl;
  return GetTouchableFromStepPoint( preSP );
}
