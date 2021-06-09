#include "GmPSZPlaneDirChecker.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosPhysics/PhysicsList/include/GmPhysicsVerbosity.hh"
#include "G4ThreeVector.hh"
#include "G4Track.hh"

//---------------------------------------------------------------------------------------
GmPSZPlaneDirChecker::GmPSZPlaneDirChecker()
{
  iUseFOI = G4int(GmParameterMgr::GetInstance()->GetNumericValue("GmPSZPlaneDirChecker:UseFOI",1));
  thePlaneX = GmParameterMgr::GetInstance()->GetNumericValue("GmPSZPlaneDirChecker:XDim",100.*mm);
  thePlaneY = GmParameterMgr::GetInstance()->GetNumericValue("GmPSZPlaneDirChecker:YDim",100.*mm);
  thePlaneZ = GmParameterMgr::GetInstance()->GetNumericValue("GmPSZPlaneDirChecker:ZPos",1000.*mm);
}


//---------------------------------------------------------------------------------------
G4bool GmPSZPlaneDirChecker::IsInPlane(const G4ThreeVector dir, const G4ThreeVector pos)
{
#ifndef GAMOS_NO_VERBOSE
  if( PhysicsVerb(debugVerb) ) G4cout << " GmPSZPlaneDirChecker::IsInPlane dir " << dir << " pos " << pos << G4endl;
#endif

  if (iUseFOI == 0) return TRUE;

  G4double diffZ = thePlaneZ - pos.z();

#ifndef GAMOS_NO_VERBOSE
#ifndef GAMOS_NO_VERBOSE
  if( PhysicsVerb(debugVerb) ) G4cout << "GmPSZPlaneDirChecker: diffZ " << diffZ << " dir.z " << dir.z() << G4endl;
#endif
#endif

  //----- Check if dir is pointing towards plane
  if ( diffZ * dir.z() <= 0 ) {
#ifndef GAMOS_NO_VERBOSE
#ifndef GAMOS_NO_VERBOSE
    if( PhysicsVerb(testVerb) ) G4cout << "GmPSZPlaneDirChecker: Rejected because direction is not towards plane " << G4endl;
#endif
#endif
    return FALSE;
  }

  //G4double xInPlane = pos.x()+diffZ*(std::tan(std::acos(dir.z())));
  G4double xInPlane = pos.x()+diffZ*dir.x()/dir.z();
#ifndef GAMOS_NO_VERBOSE
#ifndef GAMOS_NO_VERBOSE
  if( PhysicsVerb(testVerb) ) G4cout << "GmPSZPlaneDirChecker: xInPlane / posX / dirX  " << xInPlane << " / " << pos.x() << " / " << dir.x() << " " << thePlaneX << G4endl;
#endif
#endif
  if ( fabs(xInPlane) > thePlaneX ) {
#ifndef GAMOS_NO_VERBOSE
#ifndef GAMOS_NO_VERBOSE
    if( PhysicsVerb(testVerb) ) G4cout << "GmPSZPlaneDirChecker: Rejected because out of plane in X " << G4endl;
#endif
#endif
    return FALSE;
  }
  //yInPlane = pos.y()+diffZ*(std::tan(std::acos(dir.z())));
  G4double yInPlane = pos.y()+diffZ*dir.y()/dir.z();
#ifndef GAMOS_NO_VERBOSE
#ifndef GAMOS_NO_VERBOSE
  if( PhysicsVerb(testVerb) ) G4cout << "GmPSZPlaneDirChecker: / posY / dirY  " << yInPlane << " / " << pos.y() << " / " << dir.y() << " " << thePlaneY << G4endl;
#endif
#endif
  if ( fabs(yInPlane) > thePlaneY ) {
#ifndef GAMOS_NO_VERBOSE
#ifndef GAMOS_NO_VERBOSE
    if( PhysicsVerb(testVerb) ) G4cout << "GmPSZPlaneDirChecker: Rejected because out of plane in Y " << G4endl;
#endif
#endif
    return FALSE;
  }

#ifndef GAMOS_NO_VERBOSE
#ifndef GAMOS_NO_VERBOSE
  if( PhysicsVerb(testVerb) ) G4cout << "GmPSZPlaneDirChecker: Accepted xInPlane/yInPlane = " << xInPlane << " / " << yInPlane
		 << " thePlaneX/Y= " << thePlaneX << " / " << thePlaneY << G4endl;
#endif
#endif
  return TRUE;
}


//---------------------------------------------------------------------------------------
G4bool GmPSZPlaneDirChecker::IsInPlane(const G4Track* aTrack)
{
  G4ThreeVector pos = aTrack->GetPosition();
  G4ThreeVector dir = aTrack->GetMomentumDirection();
  return IsInPlane( pos, dir );
}
