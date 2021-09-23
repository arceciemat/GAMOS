#include "GmGeomConditionInMass.hh"
#include "GamosCore/GamosBase/Filters/include/GmFilterVerbosity.hh"
#include "G4VTouchable.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4TouchableHistory.hh"
#include "G4TransportationManager.hh"
#include "G4Navigator.hh"

//-------------------------------------------------------------------------
GmGeomConditionInMass::GmGeomConditionInMass()
{
  theTouchable = new G4TouchableHistory();
  theNavigator = 0;
}

//-------------------------------------------------------------------------
const G4VTouchable* GmGeomConditionInMass::GetTouchableFromTrack(const G4Track* aTrack)
{
#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionInMass::GetTouchableFromTrack return touchable " << G4endl;
#endif
//  return aTrack->GetTouchable();

  if( !theNavigator ) {
    theNavigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
  }
  theNavigator->LocateGlobalPointAndUpdateTouchable( aTrack->GetPosition(), theTouchable, false ); 

  return theTouchable;
}

//-------------------------------------------------------------------------
const G4VTouchable* GmGeomConditionInMass::GetTouchableFromStep(const G4Step* aStep)
{
#ifndef GAMOS_NO_VERBOSE
  if( FilterVerb(debugVerb) ) G4cout << " GmGeomConditionInMass::GetTouchableFromTrack return touchable " << G4endl;
#endif

  if( !theNavigator ) {
    theNavigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
  }
  theNavigator->LocateGlobalPointAndUpdateTouchable( aStep->GetPreStepPoint()->GetPosition(), theTouchable, false ); 
  /*  const G4StepPoint* preSP = aStep->GetPreStepPoint();
  const G4VTouchable* touch = preSP->GetTouchable();
  G4cout << " GmGeomConditionI touch " << touch << " new " << aStep->GetTrack()->GetTouchable() << G4endl; // different if parallel geom  */
  return theTouchable;

}
