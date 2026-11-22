#include "GmVDataInitialMaterial.hh"

#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVParameterised.hh"
#include "G4VPVParameterisation.hh"

//----------------------------------------------------------------
G4Material* GmVDataInitialMaterial::GetMateFromPV( G4VPhysicalVolume* pv )
{
  // needs special treatment if track origin was in parameterised volume
  if( !pv->IsParameterised() ) {
    return pv->GetLogicalVolume()->GetMaterial();
  } else {
    return ((G4PVParameterised*)pv)->GetParameterisation()->ComputeMaterial(pv->GetCopyNo(),pv);
  }

}
