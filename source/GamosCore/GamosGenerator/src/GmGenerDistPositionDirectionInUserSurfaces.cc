#include "GmGenerDistPositionDirectionInUserSurfaces.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosGenerator/include/GmParticleSource.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "GamosCore/GamosGeometry/include/GmTouchable.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "GamosCore/GamosBase/Base/include/GmParameterMgr.hh"
#include "CLHEP/Random/RandFlat.h"
#include "G4VSolid.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Ellipsoid.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"

//------------------------------------------------------------------------
GmGenerDistPositionDirectionInUserSurfaces::GmGenerDistPositionDirectionInUserSurfaces()
{
  bOutward=GmParameterMgr::GetInstance()->GetNumericValue("GmGenerDistPositionDirectionInUserSurfaces:Outward",0);
}

//------------------------------------------------------------------------
void GmGenerDistPositionDirectionInUserSurfaces::SetParams( const std::vector<G4String>& params )
{
  GmVGenerDistPositionVolumesAndSurfaces::SetParams(params);

  unsigned int iParams = 0;
  theTranslation = G4ThreeVector( GmGenUtils::GetValue(params[iParams]), GmGenUtils::GetValue(params[iParams+1]), GmGenUtils::GetValue(params[iParams+2]) );
  theRotation = new G4RotationMatrix;
  theRotation->rotateX( GmGenUtils::GetValue(params[iParams+3]) );
  theRotation->rotateY( GmGenUtils::GetValue(params[iParams+4]) );
  theRotation->rotateZ( GmGenUtils::GetValue(params[iParams+5]) );
  
} 

//------------------------------------------------------------------------
G4ThreeVector GmGenerDistPositionDirectionInUserSurfaces::GenerateDirection( const GmParticleSource* source)
{
  G4ThreeVector dir = -source->GetPosition();
  if( theRotation ) {
    //  dir = (*(theRotation->Inverse()) ) * dir;
    dir = CLHEP::inverseOf(*theRotation) * dir;
  }
  dir -= theTranslation;

  if( bOutward ) dir *= -1;
  
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistPositionDirectionInUserSurfaces dir " << dir << G4endl; 
#endif

  return dir;

}
