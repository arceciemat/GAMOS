#include "GmGeomConditionStart.hh"
#include "G4VTouchable.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "GamosCore/GamosBase/Base/include/GmBaseVerbosity.hh"

//-------------------------------------------------------------------------
const G4VTouchable* GmGeomConditionStart::GetTouchableFromTrack(const G4Track* aTrack)
{
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << " GmGeomConditionStart::GetTouchableFromTrack step number " << aTrack->GetCurrentStepNumber() << G4endl;
#endif
  if( aTrack->GetCurrentStepNumber() != 0 ) {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmGeomConditionStart::GetTouchableFromTrack return 0 " << aTrack->GetCurrentStepNumber() << G4endl;
#endif
    return 0;
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmGeomConditionStart::GetTouchableFromTrack return touchable " << aTrack->GetTouchable()->GetVolume()->GetName() << G4endl;
#endif
    return aTrack->GetTouchable();
  }
}

//-------------------------------------------------------------------------
const G4VTouchable* GmGeomConditionStart::GetTouchableFromStep(const G4Step* aStep)
{
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << " GmGeomConditionStart::GetTouchableFromStep step number " << aStep->GetTrack()->GetCurrentStepNumber() << G4endl;
#endif
  if( aStep->GetTrack()->GetCurrentStepNumber() != 1 ) {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmGeomConditionStart::GetTouchableFromStep return 0 " << G4endl;
#endif
    return 0;
  } else {
    const G4StepPoint* preSP = aStep->GetPreStepPoint();
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmGeomConditionStart::GetTouchableFromStep return ";
#endif
    if( preSP->GetTouchable() && preSP->GetTouchable()->GetVolume() ) {
#ifndef GAMOS_NO_VERBOSE
      if( BaseVerb(debugVerb) ) G4cout << preSP->GetTouchable()->GetVolume()->GetName() << G4endl; 
#endif
    } else {
#ifndef GAMOS_NO_VERBOSE
      if( BaseVerb(debugVerb) ) G4cout << " 0 " << G4endl;
#endif
    }
    return GetTouchableFromStepPoint( preSP );
  }
}
