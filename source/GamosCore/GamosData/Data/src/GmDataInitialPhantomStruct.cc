#include "GmDataInitialPhantomStruct.hh"
#include "GamosCore/GamosReadDICOM/include/GmReadPhantomStMgr.hh"

#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "GamosCore/GamosData/Management/include/GmDataVerbosity.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VPhysicalVolume.hh"
#include "G4TouchableHistory.hh"
#include "G4TransportationManager.hh"
#include "G4Navigator.hh"
#include "G4Event.hh"

//----------------------------------------------------------------
GmDataInitialPhantomStruct::GmDataInitialPhantomStruct()
{
  bInitial = true;
  theNChar = 25;
  theNBytes = theNChar;

  theStructMgr = GmReadPhantomStMgr::GetInstance();
  theGeomUtils = GmGeometryUtils::GetInstance();

  //  theExcludedTypes.insert(DTSeco);
}

//----------------------------------------------------------------
GmDataInitialPhantomStruct::~GmDataInitialPhantomStruct()
{
}

//----------------------------------------------------------------
G4String GmDataInitialPhantomStruct::GetStringValueFromStep( const G4Step* aStep )
{ 
  const G4VTouchable* touch = aStep->GetPreStepPoint()->GetTouchable();
  
  return GetStructNames(touch);
}

//----------------------------------------------------------------
G4String GmDataInitialPhantomStruct::GetStringValueFromTrack( const G4Track* aTrack )
{ 
  const G4VTouchable* touch = aTrack->GetStep()->GetPreStepPoint()->GetTouchable();
  
  return GetStructNames(touch);
}

//----------------------------------------------------------------
G4String GmDataInitialPhantomStruct::GetStringValueFromSecoTrack( const G4Track* aTrack1, const G4Track* )
{
  const G4VTouchable* touch = aTrack1->GetStep()->GetPreStepPoint()->GetTouchable();

  return GetStructNames(touch);
}

//----------------------------------------------------------------
G4String GmDataInitialPhantomStruct::GetStringValueFromEvent( const G4Event* anEvent )
{ 
  G4TouchableHistory* touch = new G4TouchableHistory;
  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndUpdateTouchable( anEvent->GetPrimaryVertex(0)->GetPosition(), touch, false ); 

  return GetStructNames(touch);
   
  delete touch;
}

//----------------------------------------------------------------
G4String GmDataInitialPhantomStruct::GetStringValueFromStackedTrack( const G4Track* aTrack )
{
  G4TouchableHistory* touch = new G4TouchableHistory;
  G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndUpdateTouchable( aTrack->GetPosition(), touch, false ); 

  return GetStructNames(touch);
   
  delete touch;

}

//----------------------------------------------------------------
G4String GmDataInitialPhantomStruct::GetStructNames(const G4VTouchable* touch)
{
  G4String stName = "";
  G4VPhysicalVolume* pv = touch->GetVolume();
  if( !pv ) {
    return "NO_STRUCT";
  }
  if( GmGeometryUtils::GetInstance()->IsPhantomVolume( pv ) ){
    G4int idx = touch->GetReplicaNumber();
    if( idx != -1 ) {
#ifndef GAMOS_NO_VERBOSE
      if( DataVerb(debugVerb) ) G4cout << pv->GetCopyNo() << " GmDataInitialPhantomStruct idx " << idx << " " << theStructMgr->GetStNameList(idx) << G4endl;
#endif
      std::set<size_t> StIDList = theStructMgr->GetStIDList(idx);
      for( std::set<size_t>::const_iterator ite = StIDList.begin(); ite != StIDList.end(); ite++ ) {
	G4int idx1 = *ite;
	if( stName != "" ) stName += ":";
	stName += theStructMgr->GetStName(idx1);
      }
    } else {
      stName = "NO_STRUCT";
    }
  } else {
    stName = "NO_STRUCT";
  }
  return stName;

}
