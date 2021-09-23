#include "GmGeomConditionEnd.hh"
#include "G4VTouchable.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "GamosCore/GamosBase/Filters/include/GmFilterVerbosity.hh"

//-------------------------------------------------------------------------
const G4VTouchable* GmGeomConditionEnd::GetTouchableFromTrack(const G4Track* aTrack)
{
#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionEnd::GetTouchableFromTrack track status " << aTrack->GetTrackStatus() << G4endl;
#endif
  // check not exiting world
  if( aTrack->GetVolume() == 0 ) {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionEnd::GetTouchableFromTrack return 0 " << aTrack->GetTrackStatus() << G4endl;
#endif
    return 0;
  }

  if( aTrack->GetTrackStatus() != fStopAndKill ) {
    return 0;
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionEnd::GetTouchableFromTrack return 0 " << aTrack->GetTrackStatus() << G4endl;
#endif
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionEnd::GetTouchableFromTrack return touchable ";
#endif
    if( aTrack->GetTouchable() &&  aTrack->GetTouchable()->GetVolume() ) {
#ifndef GAMOS_NO_VERBOSE
      if( FilterVerb(debugVerb) ) G4cout << aTrack->GetTouchable()->GetVolume()->GetName() << G4endl; 
#endif
    } else {
#ifndef GAMOS_NO_VERBOSE
      if( FilterVerb(debugVerb) ) G4cout << " 0 " << G4endl;
#endif
    }
    
    return aTrack->GetTouchable();
  }
}

//-------------------------------------------------------------------------
const G4VTouchable* GmGeomConditionEnd::GetTouchableFromStep(const G4Step* aStep)
{
#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionEnd::GetTouchableFromStep track status " << aStep->GetTrack()->GetTrackStatus() << G4endl;
#endif
  if( aStep->GetTrack()->GetTrackStatus() != fStopAndKill ) {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionEnd::GetTouchableFromStep return 0 " << aStep->GetTrack()->GetTrackStatus() << G4endl;
#endif
    return 0;
  } else {
    const G4StepPoint* preSP = aStep->GetPreStepPoint();
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionEnd::GetTouchableFromStep return ";
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
}

