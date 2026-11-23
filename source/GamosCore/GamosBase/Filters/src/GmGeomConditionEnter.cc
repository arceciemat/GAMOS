#include "GmGeomConditionEnter.hh"
#include "G4VTouchable.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "GamosCore/GamosBase/Filters/include/GmFilterVerbosity.hh"

//-------------------------------------------------------------------------
const G4VTouchable* GmGeomConditionEnter::GetTouchableFromTrack(const G4Track* )
{
#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionEnter::GetTouchableFromTrack  return 0 " << G4endl;
#endif
  // it cannot be entering any volume if it is PreUserTrackingAction or PostUserTrackingAction
  return 0;
}

//-------------------------------------------------------------------------
const G4VTouchable* GmGeomConditionEnter::GetTouchableFromStep(const G4Step* step)
{
  const G4StepPoint* postSP = step->GetPostStepPoint();

  if( postSP->GetStepStatus() != fGeomBoundary ) {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionEnter::GetTouchableFromStep stepStatus is " << postSP->GetStepStatus() << " not GeomBoundary, return 0 " << G4endl;
#endif
    return 0;
  } else {
#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionEnter::GetTouchableFromStep stepStatus is GeomBoundary, return touchable " << G4endl;
#endif
    return GetTouchableFromStepPoint( postSP );
  }
}


