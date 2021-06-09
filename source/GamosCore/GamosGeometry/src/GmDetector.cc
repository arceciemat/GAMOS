#include "GmDetector.hh"

#include "G4Transform3D.hh"

#include "GmTouchable.hh"
#include "GmSolid.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
//#define DEBUG

#ifdef DEBUG
#include "G4VPhysicalVolume.hh"
#include "G4TransportationManager.hh"
#include "G4Navigator.hh"
#endif

//-----------------------------------------------------------------------
GmDetector::GmDetector( GmSolid* sol, const G4ThreeVector& pos, const G4RotationMatrix& rotmat )
{
  theName = sol->GetName();
  theTransform = G4Transform3D( rotmat, pos );
  theSolid = sol; 
  theID = 0;
}


//-----------------------------------------------------------------------
GmDetector::GmDetector( GmSolid* sol, const G4Transform3D& transf )
{
  theName = sol->GetName();
  theTransform = transf;
  theSolid = sol; 
  theID = 0;
}


//-----------------------------------------------------------------------
GmDetector::GmDetector( GmTouchable* to )
{
  theName = to->GetLongName();
  theTransform = G4Transform3D( to->GetGlobalRotation(), to->GetGlobalPosition() );
  theSolid = new GmSolid( (to->GetSolid()) );
  theID = to->GetCopyNo();

}

//-----------------------------------------------------------------------
GmDetector::~GmDetector()
{
}

//------------------------------------------------------------------------
G4Point3D GmDetector::GetRandomPoint() const
{
  G4Point3D point = theSolid->GetRandomPoint();
  //  G4cout << " GmDetector::GetRandomPoint bef transf " << point << G4endl;
  point = point.transform( theTransform );
  //  G4cout << " GmDetector::GetRandomPoint aft transf " << point << " transf " << theTransform.getRotation() << G4endl;

#ifdef DEBUG
  G4VPhysicalVolume* pv = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndSetup( point, (const G4ThreeVector*)0,false, true );
  G4int isl = theName.rfind("/");
  G4String name;
  if( isl != -1 ) {
    name = theName.substr( isl+1, theName.length() );
  } else {
    name = theName;
  } 
  if( pv->GetName()+":"+GmGenUtils::itoa(pv->GetCopyNo()) != name ) {
    G4cerr << " GmDetector::GetRandomPoint " << pv->GetName()+":"+GmGenUtils::itoa(pv->GetCopyNo()) << " is not inside detector " << name << G4endl;
  }
#endif

  return point;

}

//------------------------------------------------------------------------
G4Point3D GmDetector::GetCentrePoint() const
{
  G4Point3D point(0.,0.,0.);
  //  G4cout << " GmDetector::GetRandomPoint bef transf " << point << G4endl;
  point = point.transform( theTransform );
  //  G4cout << " GmDetector::GetRandomPoint aft transf " << point << " transf " << theTransform.getRotation() << G4endl;

#ifdef DEBUG
  G4VPhysicalVolume* pv = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking()->LocateGlobalPointAndSetup( point, (const G4ThreeVector*)0,false, true );
  G4int isl = theName.rfind("/");
  G4String name;
  if( isl != -1 ) {
    name = theName.substr( isl+1, theName.length() );
  } else {
    name = theName;
  }
  if( pv->GetName()+":"+GmGenUtils::itoa(pv->GetCopyNo()) != name ) {
    G4cerr << " GmDetector::GetRandomPoint " << pv->GetName()+":"+GmGenUtils::itoa(pv->GetCopyNo()) << " is not inside detector " << name << G4endl;
  }
#endif

  return point;

}
