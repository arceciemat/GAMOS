#include "GmGeomConditionEnter.hh"
#include "G4VTouchable.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "GamosCore/GamosBase/Base/include/GmBaseVerbosity.hh"

//-------------------------------------------------------------------------
const G4VTouchable* GmGeomConditionEnter::GetTouchableFromTrack(const G4Track* )
{
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << " GmGeomConditionEnter::GetTouchableFromTrack  return 0 " << G4endl;
#endif
  // it cannot be entering any volume if it is PreUserTrackingAction or PostUserTrackingAction
  return 0;
}

//-------------------------------------------------------------------------
const G4VTouchable* GmGeomConditionEnter::GetTouchableFromStep(const G4Step* step)
{
  const G4StepPoint* postSP = step->GetPostStepPoint();
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << " GmGeomConditionEnter::GetTouchableFromStep stepStatus " << postSP->GetStepStatus() << G4endl;
#endif
  if( postSP->GetStepStatus() != fGeomBoundary ) {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmGeomConditionEnter::GetTouchableFromStep return 0, stepStatus " << postSP->GetStepStatus() << G4endl;
#endif
    return 0;
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmGeomConditionEnter::GetTouchableFromStep return touchable ";
#endif
    if( postSP->GetTouchable() && postSP->GetTouchable()->GetVolume() ) {
#ifndef GAMOS_NO_VERBOSE
      if( BaseVerb(debugVerb) ) G4cout << postSP->GetTouchable()->GetVolume()->GetName() << G4endl; 
#endif
    } else {
#ifndef GAMOS_NO_VERBOSE
      if( BaseVerb(debugVerb) ) G4cout << " 0 " << G4endl;
#endif
    }
    return GetTouchableFromStepPoint( postSP );
  }
}
