#include "GmVEWBiasingOperation.hh"
#include "GmBiasingVerbosity.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"

#include "G4BiasingProcessInterface.hh"

#include "G4ParticleChangeForLoss.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmVEWBiasingOperation::GmVEWBiasingOperation(G4String name)
: G4VBiasingOperation(name),
  fNSplit(1),
  fNSplitInv(1.)
  //t, fParticleChange()
{
  thePlaneX = GmParameterMgr::GetInstance()->GetNumericValue(GetName()+":XDim",100.*CLHEP::mm);
  thePlaneY = GmParameterMgr::GetInstance()->GetNumericValue(GetName()+":YDim",100.*CLHEP::mm);
  thePlaneZ = GmParameterMgr::GetInstance()->GetNumericValue(GetName()+":ZPos",1000.*CLHEP::mm);

  theGasDensity = GmParameterMgr::GetInstance()->GetNumericValue(GetName()+":GasDensity",0.012) * CLHEP::g/CLHEP::cm3;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GmVEWBiasingOperation::~GmVEWBiasingOperation()
{
  fParticleChange.Clear();
}

//---------------------------------------------------------------------------------------
G4bool GmVEWBiasingOperation::IsInPlane(const G4ThreeVector dir, const G4ThreeVector pos)
{
#ifndef GAMOS_NO_VERBOSE
  if( BiasingVerb(debugVerb) ) G4cout << " GmVEWBiasingOperation::IsInPlane dir " << dir << " pos " << pos << G4endl;
#endif

  G4double diffZ = thePlaneZ - pos.z();

#ifndef GAMOS_NO_VERBOSE
  if( BiasingVerb(debugVerb) ) G4cout << "GmVEWBiasingOperation::IsInPlane diffZ " << diffZ << " dir.z " << dir.z() << G4endl;
#endif

  //----- Check if dir is pointing towards plane
  if ( diffZ * dir.z() <= 0 ) {
#ifndef GAMOS_NO_VERBOSE
    if( BiasingVerb(testVerb) ) G4cout << "GmVEWBiasingOperation::IsInPlane Rejected because direction is not towards plane " << G4endl;
#endif
    return FALSE;
  }

  //G4double xInPlane = pos.x()+diffZ*(std::tan(std::acos(dir.z())));
  G4double xInPlane = pos.x()+diffZ*dir.x()/dir.z();
#ifndef GAMOS_NO_VERBOSE
  if( BiasingVerb(testVerb) ) G4cout << "GmVEWBiasingOperation::IsInPlane xInPlane / posX / dirX  " << xInPlane << " / " << pos.x() << " / " << dir.x() << " " << thePlaneX << G4endl;
#endif

  if ( fabs(xInPlane) > thePlaneX ) {
#ifndef GAMOS_NO_VERBOSE
    if( BiasingVerb(testVerb) ) G4cout << "GmVEWBiasingOperation::IsInPlane Rejected because out of plane in X " << G4endl;
#endif
    return FALSE;
  }
  //yInPlane = pos.y()+diffZ*(std::tan(std::acos(dir.z())));
  G4double yInPlane = pos.y()+diffZ*dir.y()/dir.z();
#ifndef GAMOS_NO_VERBOSE
  if( BiasingVerb(testVerb) ) G4cout << "GmVEWBiasingOperation::IsInPlane yInPlane / posY / dirY  " << yInPlane << " / " << pos.y() << " / " << dir.y() << " " << thePlaneY << G4endl;
#endif

  if ( fabs(yInPlane) > thePlaneY ) {
#ifndef GAMOS_NO_VERBOSE
    if( BiasingVerb(testVerb) ) G4cout << "GmVEWBiasingOperation::IsInPlane Rejected because out of plane in Y " << G4endl;
#endif
    return FALSE;
  }

#ifndef GAMOS_NO_VERBOSE
  if( BiasingVerb(testVerb) ) G4cout << "GmVEWBiasingOperation: Accepted xInPlane/yInPlane = " << xInPlane << " / " << yInPlane
		 << " thePlaneX/Y= " << thePlaneX << " / " << thePlaneY << G4endl;
#endif
  return TRUE;
}

