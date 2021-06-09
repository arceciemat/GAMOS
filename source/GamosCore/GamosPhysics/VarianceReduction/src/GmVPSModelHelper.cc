#include "GmVPSModelHelper.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#ifndef GAMOS_NO_VERBOSE
#include "GamosCore/GamosPhysics/PhysicsList/include/GmPhysicsVerbosity.hh"
#endif
#include "G4Gamma.hh"
#include "G4BremsstrahlungCrossSectionHandler.hh"
#include "CLHEP/Random/RandFlat.h"

//------------------------------------------------------------------------
GmVPSModelHelper::GmVPSModelHelper()
{
#ifndef GAMOS_NO_VERBOSE
  if( PhysicsVerb(debugVerb) ) G4cout << "In GmVPSModelHelper's Constructor..." << G4endl;
#endif

  bUseRRplane = G4int(GmParameterMgr::GetInstance()->GetNumericValue("GmEWPS:UseRRplane",0));
  bUseEplane = G4int(GmParameterMgr::GetInstance()->GetNumericValue("GmEWPS:UseEplane",0));
  zRussRoul = GmParameterMgr::GetInstance()->GetNumericValue("GmEWPS:zRRplane",0.);
  zEleSplit = GmParameterMgr::GetInstance()->GetNumericValue("GmEWPS:zEplane",0.*mm);
#ifndef GAMOS_NO_VERBOSE
  if( PhysicsVerb(infoVerb) ) G4cout << "UseRRplane " << bUseRRplane <<  " / UseEplane " << bUseEplane << " / zRRplane " << zRussRoul << " / zEplane " << zEleSplit << G4endl;
#endif
  if ( bUseRRplane == 1 && bUseEplane == 1 && zRussRoul > zEleSplit ) {
    G4Exception("GmVPSModelHelper::GmVPSModelHelper",
		"Russian Roulette plane must be above the e- splitting plane!",FatalErrorInArgument,G4String(G4String("It is below...")));
  }

  theGasMaxDensity = GmParameterMgr::GetInstance()->GetNumericValue("GmEWPS:GasMaxDensity", 0.012*g/cm3);

  theNSplit = G4int(GmParameterMgr::GetInstance()->GetNumericValue("GmParticleSplittingProcess:NSplit",10));
  theNSplitInv = 1./theNSplit;

  thePlaneChecker = new GmPSZPlaneDirChecker;

  thePlaneZ = GmParameterMgr::GetInstance()->GetNumericValue("GmPSZPlaneDirChecker:ZPos",1000.*mm);

}


//------------------------------------------------------------------------
G4bool GmVPSModelHelper::RussRoul()
{
  if ( CLHEP::RandFlat::shoot() < theNSplitInv ) {
#ifndef GAMOS_NO_VERBOSE
    if( PhysicsVerb(debugVerb) ) G4cout << "GmVPSModelHelper: Accepted by Russian Roul. and made fat. Wgt 1" << G4endl;
#endif
    return true;
  }
#ifndef GAMOS_NO_VERBOSE
  if( PhysicsVerb(debugVerb) ) G4cout << "GmVPSModelHelper: Rejected by Russian Roul." << G4endl;
#endif
  return false;
}


//------------------------------------------------------------------------
G4double GmVPSModelHelper::CheckGasNonGas(const G4Track& aTrack)
{
  /*
	If not in gas, plays Russian Roulette and returns -1 (reject) or +1 (accept)
	In gas, sets theNewNSplit to 1 (no splitting) and returns original weight of track
  */
  if (aTrack.GetMaterial()->GetDensity() > theGasMaxDensity) {
#ifndef GAMOS_NO_VERBOSE
#ifndef GAMOS_NO_VERBOSE
    if( PhysicsVerb(debugVerb) ) G4cout << "GmVPSModelHelper: Gamma is not in a gas." << G4endl;
#endif
#endif
    return RussRoul();
  }
  else 
  {
#ifndef GAMOS_NO_VERBOSE
#ifndef GAMOS_NO_VERBOSE
    if( PhysicsVerb(debugVerb) ) G4cout << "GmVPSModelHelper: Gamma in a gas. No splitting." << G4endl;
#endif
#endif
    theNewNSplit=1;
    return aTrack.GetWeight();
  }
}


//------------------------------------------------------------------------
G4bool GmVPSModelHelper::AcceptGamma(const G4ThreeVector vGammadir, const G4ThreeVector vGammapos)
{
  return thePlaneChecker->IsInPlane(vGammadir, vGammapos);
}


//------------------------------------------------------------------------
PSZone GmVPSModelHelper::GetZone(const G4double ztrack)
{
  // If RR plane is not used, treat all as in zone 1
  if (bUseRRplane == 0) {
#ifndef GAMOS_NO_VERBOSE
    if( PhysicsVerb(debugVerb) ) G4cout << " GmVPSModelHelper: RR plane not used. Using same splitting everywhere" << G4endl;
#endif
    return PSZAll;
  }
  if (ztrack < zRussRoul) {
    return PSZBeforeRR;
  } else {
    if (ztrack < zEleSplit) {
      return PSZAfterRR;
    } else {
      return PSZAfterEleSplit;
    }
  }
}



/* Another way of doing AcceptGamma
  G4double fPlaneX, fPlaneY, thePlaneZ;
  G4cout << " vGammaposX,Y,Z  " << vGammapos.x() << " " << vGammapos.y() << " " << vGammapos.z() << G4endl;
  G4cout << " vGammadirX,Y,Z  " << vGammadir << G4endl;
  fPlaneX = GmParameterMgr::GetInstance()->GetNumericValue("GmPSZPlaneDirChecker:XDim",100.*mm);
  fPlaneY = GmParameterMgr::GetInstance()->GetNumericValue("GmPSZPlaneDirChecker:YDim",100.*mm);
  thePlaneZ = GmParameterMgr::GetInstance()->GetNumericValue("GmPSZPlaneDirChecker:ZPos",1000.*mm);

  The normal to the plane (which is itself perpendicular to the z axis) is n_p = (0,0,-1)
  The centre of the plane is at position V_0 = (0,0,thePlaneZ)
  The initial position of gamma is R_0 = (vGammaPosX,vGammaPosY,vGammaPosZ)
  The unitary vector of the gamma's direction is
  u_g =    (vGammaDirX, vGammaDirY, vGammaDirZ)
          --------------------------------------------
          (vGammaDirX^2 + vGammaDirY^2 + vGammaDirZ^2)
  The intersection of the plane with the photon's vector is at the point S_I given by
  S_I = n_p (dot) (V_0 - R_0)
        --------------------
           n_p (dot) u_g
  There is an intersection with the plane if S_I >=0
  Finally, the position of the intersection is at R = (Rx,Ry,Rz) = R_0 + (S_I)*u_g

  G4double ModGammadir,S_I;
  ModGammadir = std::sqrt(vGammadir.x()*vGammadir.x() + vGammadir.y()*vGammadir.y()
                          + vGammadir.z()*vGammadir.z());
  
  S_I = (thePlaneZ-vGammapos.z())*ModGammadir/vGammadir.z();
  G4cout << " S_I " << S_I << G4endl;
  if (S_I<0.0) return FALSE;

  G4double ModInvGammadir,Rx;
  ModInvGammadir = 1.0/ModGammadir;
  if (vGammadir.z()==0) { G4cout << " Gamma rejected. It has pz=0" << G4endl; return FALSE; }
  else G4cout << " Gamma with pz not equal to 0.";
  Rx = vGammapos.x() + S_I*(vGammadir.x()*ModInvGammadir);
  G4cout << " Rx " << Rx << G4endl;
  if (fabs(Rx) > fPlaneX) { G4cout << " Rejected by x" << G4endl; return FALSE; }

  G4double Ry;
  Ry = vGammapos.y() + S_I*(vGammadir.y()*ModInvGammadir);
  G4cout << " Ry " << Ry << G4endl;
  if (fabs(Ry) > fPlaneY) { G4cout << " Rejected by y" << G4endl; return FALSE; }

  G4double Rz;
  Rz = vGammapos.z() + S_I*(vGammadir.z()*ModInvGammadir);
  G4cout << " Rz " << Rz << G4endl;
  if (fabs(Rz) > thePlaneZ) { G4cout << " Rejected by z" << G4endl; return FALSE; }

  return TRUE;
*/

/*
  G4double diffZ = thePlaneZ - vGammapos.z();

  //----- Check if dir is pointing towards plane
  //if ( diffZ * vGammadir.z() <= 0 ) { G4cout << " Rejected by z" << G4endl; return FALSE; }

  G4double xInPlane = vGammapos.x()+vGammadir.x()*diffZ;
  if ( fabs(xInPlane) > fPlaneX )  { G4cout << " Rejected by x" << G4endl; return FALSE; }

  G4double yInPlane = vGammapos.y()+vGammadir.y()*diffZ;
  if ( fabs(yInPlane) > fPlaneY )  { G4cout << " Rejected by y" << G4endl; return FALSE; }

  return TRUE;
*/


//------------------------------------------------------------------------
G4bool GmVPSModelHelper::IsApplicable(G4double posZ)
{
  if( posZ < thePlaneZ ) {
    return true;
  } else {
    return false;
  }
}
