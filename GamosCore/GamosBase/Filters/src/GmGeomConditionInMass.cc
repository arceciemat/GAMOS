#include "GmGeomConditionInMass.hh"
#include "GamosCore/GamosBase/Base/include/GmBaseVerbosity.hh"
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
}

//-------------------------------------------------------------------------
const G4VTouchable* GmGeomConditionInMass::GetTouchableFromTrack(const G4Track* aTrack)
{
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << " GmGeomConditionInMass::GetTouchableFromTrack return touchable " << G4endl;
#endif
//  return aTrack->GetTouchable();

  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndUpdateTouchable( aTrack->GetPosition(), theTouchable, false ); 

  return theTouchable;
}

//-------------------------------------------------------------------------
const G4VTouchable* GmGeomConditionInMass::GetTouchableFromStep(const G4Step* aStep)
{
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << " GmGeomConditionInMass::GetTouchableFromTrack return touchable " << G4endl;
#endif

  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndUpdateTouchable( aStep->GetPreStepPoint()->GetPosition(), theTouchable, false ); 
  /*  const G4StepPoint* preSP = aStep->GetPreStepPoint();
  const G4VTouchable* touch = preSP->GetTouchable();
  G4cout << " GmGeomConditionI touch " << touch << " new " << aStep->GetTrack()->GetTouchable() << G4endl; // different if parallel geom  */
  return theTouchable;

}
