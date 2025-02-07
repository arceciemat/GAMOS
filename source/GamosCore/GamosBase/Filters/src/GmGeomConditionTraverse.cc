#include "GmGeomConditionTraverse.hh"
#include "G4VTouchable.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "GamosCore/GamosBase/Filters/include/GmFilterVerbosity.hh"

//-------------------------------------------------------------------------
const G4VTouchable* GmGeomConditionTraverse::GetTouchableFromTrack(const G4Track* aTrack)
{
#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionTraverse::GetTouchableFromStep " << G4endl;
#endif
  // check it is in volume
  // check not exiting world
  if( aTrack->GetVolume() == 0 ) {
#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionTraverse::GetTouchableFromStep return 0 " << G4endl;
#endif
    return 0;
  }

#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionTraverse::GetTouchableFromStep return touchable ";
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

//-------------------------------------------------------------------------
const G4VTouchable* GmGeomConditionTraverse::GetTouchableFromStep(const G4Step* aStep)
{
#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionTraverse::GetTouchableFromStep " << G4endl;
#endif
  const G4StepPoint* preSP = aStep->GetPreStepPoint();
  const G4StepPoint* postSP = aStep->GetPostStepPoint();
  if( preSP->GetTouchable() != postSP->GetTouchable() ) {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionTraverse::GetTouchableFromStep return 0  preSP " << preSP->GetTouchable() << " != " << postSP->GetTouchable() << G4endl;
#endif
    return 0;
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionTraverse::GetTouchableFromStep return touchable ";
    if( preSP->GetPhysicalVolume() && preSP->GetPhysicalVolume()->GetName() ) {
      if( FilterVerb(debugVerb) ) G4cout << preSP->GetPhysicalVolume()->GetName() << G4endl;
    } else {
      if( FilterVerb(debugVerb) ) G4cout << " 0 " << G4endl;
    }
#endif
    return GetTouchableFromStepPoint( preSP );
  }
}

