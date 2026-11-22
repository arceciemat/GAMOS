#include "GmGeomConditionStart.hh"
#include "G4VTouchable.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4TransportationManager.hh"
#include "G4Navigator.hh"
#include "G4TouchableHistory.hh"
#include "GamosCore/GamosBase/Filters/include/GmFilterVerbosity.hh"

GmGeomConditionStart::GmGeomConditionStart()
{
  theTouchable = new G4TouchableHistory();
  theNavigator = 0;
}

//-------------------------------------------------------------------------
const G4VTouchable* GmGeomConditionStart::GetTouchableFromTrack(const G4Track* aTrack)
{
#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionStart::GetTouchableFromTrack step number " << aTrack->GetCurrentStepNumber() << G4endl;
#endif
  if( !theNavigator ) {
    theNavigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
  }
  theNavigator->LocateGlobalPointAndUpdateTouchable( aTrack->GetVertexPosition(), theTouchable, false ); 

#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionStart::GetTouchableFromTrack return touchable " << theTouchable->GetVolume()->GetName() << " POS " << aTrack->GetVertexPosition() << G4endl;
#endif
  return theTouchable;
}

//-------------------------------------------------------------------------
const G4VTouchable* GmGeomConditionStart::GetTouchableFromStep(const G4Step* aStep)
{
#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionStart::GetTouchableFromStep step number " << aStep->GetTrack()->GetCurrentStepNumber() << G4endl;
#endif
  if( aStep->GetTrack()->GetCurrentStepNumber() != 1 ) {
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionStart::GetTouchableFromStep return 0 " << G4endl;
#endif
    return 0;
  } else {
    const G4StepPoint* preSP = aStep->GetPreStepPoint();
#ifndef GAMOS_NO_VERBOSE
    if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionStart::GetTouchableFromStep return ";
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

//-------------------------------------------------------------------------
const G4VTouchable* GmGeomConditionStart::GetTouchableFromStackedTrack(const G4Track* aTrack)
{
#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionStart::GetTouchableFromStackedTrack step number " << aTrack->GetCurrentStepNumber() << G4endl;
#endif
  if( !theNavigator ) {
    theNavigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
  }
  theNavigator->LocateGlobalPointAndUpdateTouchable( aTrack->GetPosition(), theTouchable, false ); 

#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionStart::GetTouchableFromStackedTrack return touchable " << theTouchable->GetVolume()->GetName() << " POS " << aTrack->GetPosition() << G4endl;
#endif
  return theTouchable;
}
