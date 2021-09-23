#include "GmGeomConditionExit.hh"
#include "G4VTouchable.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "GamosCore/GamosBase/Filters/include/GmFilterVerbosity.hh"

//-------------------------------------------------------------------------
const G4VTouchable* GmGeomConditionExit::GetTouchableFromTrack(const G4Track* trk)
{
#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionExit::GetTouchableFromTrack" << G4endl;
#endif

  // it cannot be exiting any volume if it is PreUserTrackingAction or PostUserTrackingAction, unless it is exiting the world
  if( trk->GetTrackStatus() == fStopAndKill ) {
    if( trk->GetStep() != 0 ) {
      const G4StepPoint* preSP = trk->GetStep()->GetPreStepPoint();
      const G4StepPoint* postSP = trk->GetStep()->GetPostStepPoint();
      if( postSP->GetStepStatus() == fWorldBoundary ){ 
#ifndef GAMOS_NO_VERBOSE
	if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionExit::GetTouchableFromTrack return touchable " << preSP->GetTouchable()->GetVolume()->GetName() << G4endl;
#endif
	return GetTouchableFromStepPoint( preSP );
      }
    }
  }

#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionExit::GetTouchableFromTrack return 0 " << G4endl;
#endif

  return 0;
}

//-------------------------------------------------------------------------
const G4VTouchable* GmGeomConditionExit::GetTouchableFromStep(const G4Step* step)
{
  const G4StepPoint* preSP = step->GetPreStepPoint();
  const G4StepPoint* postSP = step->GetPostStepPoint();
  if( postSP->GetStepStatus() != fGeomBoundary ) {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionExit::GetTouchableFromStep stepStatus is " << postSP->GetStepStatus() << " not GeomBoundary, return 0 " << G4endl;
#endif
    return 0;
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionExit::GetTouchableFromStep stepStatus is GeomBoundary, return touchable " << G4endl;
#endif
    return GetTouchableFromStepPoint( preSP );
  }

  return 0;

}


