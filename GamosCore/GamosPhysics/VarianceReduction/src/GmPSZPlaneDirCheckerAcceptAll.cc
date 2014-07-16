#include "GmPSZPlaneDirCheckerAcceptAll.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "GamosCore/GamosPhysics/PhysicsList/include/GmPhysicsVerbosity.hh"
#include "G4ThreeVector.hh"
#include "G4Track.hh"

//---------------------------------------------------------------------------
GmPSZPlaneDirCheckerAcceptAll::GmPSZPlaneDirCheckerAcceptAll()
{
}


//---------------------------------------------------------------------------
G4bool GmPSZPlaneDirCheckerAcceptAll::IsInPlane(const G4ThreeVector , const G4ThreeVector )
{
  return TRUE;

}


//---------------------------------------------------------------------------
G4bool GmPSZPlaneDirCheckerAcceptAll::IsInPlane(const G4Track* )
{
  return TRUE;
}
