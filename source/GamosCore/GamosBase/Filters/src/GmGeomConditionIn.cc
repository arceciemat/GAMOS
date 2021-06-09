#include "GmGeomConditionIn.hh"
#include "GamosCore/GamosBase/Base/include/GmBaseVerbosity.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4Step.hh"

//-------------------------------------------------------------------------
GmGeomConditionIn::GmGeomConditionIn()
{
  theTouchable = new G4TouchableHistory();
}

//-------------------------------------------------------------------------
const G4VTouchable* GmGeomConditionIn::GetTouchableFromTrack(const G4Track* aTrack)
{
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << " GmGeomConditionIn::GetTouchableFromTrack " << G4endl;
#endif
  // check not exiting world
  if( aTrack->GetVolume() == 0 ) {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << " GmGeomConditionIn::GetTouchableFromTrack return 0 " << G4endl;
#endif
    return 0;
  }

#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << " GmGeomConditionIn::GetTouchableFromTrack return touchable " 
				   << "PARALLEL_WORLD: " << aTrack->GetTouchable()->GetVolume()->GetName() 
				   << " REAL_WORLD: " << aTrack->GetVolume()->GetName() << G4endl;
#endif
  return aTrack->GetTouchable();

}

#include "G4TouchableHistory.hh"
#include "G4TransportationManager.hh"
#include "G4Navigator.hh"

//-------------------------------------------------------------------------
const G4VTouchable* GmGeomConditionIn::GetTouchableFromStep(const G4Step* aStep)
{
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << " GmGeomConditionIn::GetTouchableFromStep " << G4endl;
#endif

  const G4StepPoint* preSP = aStep->GetPreStepPoint();
#ifndef GAMOS_NO_VERBOSE
  if( BaseVerb(debugVerb) ) G4cout << "GmGeomConditionIn::GetTouchableFromStep return touchable ";
#endif
  if( preSP->GetTouchable() && preSP->GetTouchable()->GetVolume() ) {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) {
   	  if( preSP->GetTouchable()->GetVolume() ) {
		G4cout << "PARALLEL_WORLD: " << preSP->GetTouchable()->GetVolume()->GetName();
	  }
	  if( aStep->GetTrack()->GetVolume() ) {
        G4cout << " REAL_WORLD: " << aStep->GetTrack()->GetVolume()->GetName() << G4endl;
	  }
	}
#endif
  } else {
#ifndef GAMOS_NO_VERBOSE
    if( BaseVerb(debugVerb) ) G4cout << G4endl;
#endif
  }

  //  G4cout << "GmGeomConditionIn::GetTouchableFromStep parallel volume= " << preSP->GetTouchable()->GetVolume()->GetName() << " == " << preSP->GetPhysicalVolume()->GetName() << " mass volume= " << aStep->GetTrack()->GetVolume()->GetName() << G4endl;
  return GetTouchableFromStepPoint( preSP );
}
