#include "GmDataFinalPhantomStruct.hh"
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
GmDataFinalPhantomStruct::GmDataFinalPhantomStruct()
{
  bInitial = true;
  theNChar = 25;
  theNBytes = theNChar;

  theStructMgr = GmReadPhantomStMgr::GetInstance();
  theGeomUtils = GmGeometryUtils::GetInstance();

  //  theExcludedTypes.insert(DTSeco);
}

//----------------------------------------------------------------
GmDataFinalPhantomStruct::~GmDataFinalPhantomStruct()
{
}

//----------------------------------------------------------------
G4String GmDataFinalPhantomStruct::GetStringValueFromStep( const G4Step* aStep )
{ 
  const G4VTouchable* touch = aStep->GetPostStepPoint()->GetTouchable();
  
  return GetStructNames(touch);
}

//----------------------------------------------------------------
G4String GmDataFinalPhantomStruct::GetStringValueFromTrack( const G4Track* aTrack )
{ 
  const G4VTouchable* touch = aTrack->GetStep()->GetPostStepPoint()->GetTouchable();
  
  return GetStructNames(touch);
}

//----------------------------------------------------------------
G4String GmDataFinalPhantomStruct::GetStructNames(const G4VTouchable* touch)
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
      if( DataVerb(debugVerb) ) G4cout << pv->GetCopyNo() << " GmDataFinalPhantomStruct idx " << idx << " " << theStructMgr->GetStNameList(idx) << G4endl;
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
