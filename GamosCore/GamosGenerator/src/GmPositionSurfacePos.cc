#define _USE_MATH_DEFINES
#include <cmath>
#include "GmPositionSurfacePos.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosGenerator/include/GmParticleSource.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryUtils.hh"
#include "GamosCore/GamosGeometry/include/GmTouchable.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "CLHEP/Random/RandFlat.h"
#include "G4VSolid.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4GeometryTolerance.hh"

//------------------------------------------------------------------------
GmPositionSurfacePos::GmPositionSurfacePos()
{
}

//------------------------------------------------------------------------
G4double GmPositionSurfacePos::GetObjectDimension(const G4VSolid* solid) const
{
  G4VSolid* solidnc = const_cast<G4VSolid*>(solid);
  return solidnc->GetSurfaceArea();
}

//------------------------------------------------------------------------
G4ThreeVector GmPositionSurfacePos::GeneratePosition( const GVSTouchableInfo* tinfo )
{
  G4ThreeVector pos = GeneratePosInSolidSurface( tinfo->solid );

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistPositionInG4Volumes::Generate pos before trans " << pos << G4endl;
#endif

  return pos;
}

//------------------------------------------------------------------------
G4ThreeVector GmPositionSurfacePos::GeneratePosInSolidSurface( const G4VSolid* solid )
{
  G4double angTolerance = G4GeometryTolerance::GetInstance()->GetAngularTolerance();

  G4ThreeVector position;
  
  if( solid->GetEntityType() == "G4Orb" ) {
    G4Orb* solidc = (G4Orb*)solid;
    G4double radius = solidc->GetRadius();

    // Generate (x,y,z) in sphere surface
    G4double costheta = 2*CLHEP::RandFlat::shoot()-1.;
    G4double sintheta = sqrt(1.-costheta*costheta);
    G4double phi = CLHEP::RandFlat::shoot()*CLHEP::twopi;
    position = G4ThreeVector( radius*sintheta*cos(phi), radius*sintheta*sin(phi), radius*costheta);

  } else if( solid->GetEntityType() == "G4Sphere" ) {
    G4Sphere* solidc = (G4Sphere*)solid;
    G4double radiusI = solidc->GetInsideRadius();
    G4double radiusO = solidc->GetOuterRadius();

    //----- Generate (x,y,z) in sphere surface

    G4double costhetaStart = cos( solidc->GetStartThetaAngle() );
    G4double costhetaEnd = cos( solidc->GetStartThetaAngle() + solidc->GetDeltaThetaAngle() );
    G4double phiStart = solidc->GetStartPhiAngle();
    G4double phiEnd = solidc->GetStartPhiAngle() + solidc->GetDeltaPhiAngle();
    if( phiEnd < 0. || phiEnd > CLHEP::twopi ) phiEnd = fmod(phiEnd+CLHEP::twopi,CLHEP::twopi);
    G4double costheta;
    G4double phi;
    //--- most cases it will be a full sphere, and not checking will spare some time
    G4bool bFullSphere = (radiusI == 0. && phiStart == 0. && fabs(phiEnd - CLHEP::twopi) < angTolerance && costhetaStart == 1. && fabs(costhetaEnd+1.) < angTolerance);
#ifndef GAMOS_NO_VERBOSE
    if( !bFullSphere && GenerVerb(debugVerb) ) G4cout << "GmPositionSurfacePos::GeneratePosInSolid solid is sphere but not full " << solid->GetName() << G4endl;
#endif

    for( ;; ){
      costheta = 2*CLHEP::RandFlat::shoot()-1.;
      phi = CLHEP::RandFlat::shoot()*CLHEP::twopi;
      if( !bFullSphere && (phi < phiStart || phi > phiEnd || costheta < costhetaStart || costheta > costhetaEnd) ){
	continue;
      }
      break;
    }
    G4double sintheta = sqrt(1.-costheta*costheta); 
    unsigned int ia = GetOneArea( solid );
    G4double radius = 0.;
    if(ia == 0 ) {
      radius = radiusI;
    } else if(ia == 1 ) {
      radius = radiusO;
    }

    position = G4ThreeVector( radius*sintheta*cos(phi), radius*sintheta*sin(phi), radius*costheta);

  } else if( solid->GetEntityType() == "G4Tubs" ) {
    const G4Tubs* solidc = static_cast<const G4Tubs*>(solid);

    // Select surface cylinder or endcap
    //-  G4cout << "rvolshoot " << rvol << " " <<  theTotalContainer[siz-1] << G4endl;
    unsigned int ia = GetOneArea( solid );

    G4double radius = 0.;
    G4double z = 0.;
    G4double phi;
    G4double zlength = solidc->GetZHalfLength();
    G4double phiStart = solidc->GetStartPhiAngle();
    G4double phiEnd = solidc->GetStartPhiAngle() + solidc->GetDeltaPhiAngle();
    if( phiEnd < 0. || phiEnd > CLHEP::twopi ) phiEnd = fmod(phiEnd+CLHEP::twopi,CLHEP::twopi);
    G4double radiusInner = solidc->GetInnerRadius();
    G4double radiusOuter = solidc->GetOuterRadius();
    G4bool bFullTubs = (radiusInner == 0. && phiStart == 0. && fabs(phiEnd - CLHEP::twopi) < angTolerance);

    for( ;; ){
      phi = CLHEP::RandFlat::shoot()*CLHEP::twopi;
      if(!bFullTubs && (phi < phiStart || phi > phiEnd) ){
	continue;
      }
      break;
    }

    if( ia == 0 ) { // cylinder inner
      radius = radiusInner;
      z = CLHEP::RandFlat::shoot()*2.*zlength- zlength;
    }else if(ia == 1 ) { // cylinder outer
      radius = radiusOuter;
      z = CLHEP::RandFlat::shoot()*2.*zlength- zlength;
    } else if(ia == 2 ) { // endcap negative
      radius = sqrt(CLHEP::RandFlat::shoot()*(radiusOuter*radiusOuter-radiusInner*radiusInner) - (radiusInner*radiusInner));
      z = -zlength;
    } else if(ia == 3 ) { // endcap positive
      radius = sqrt(CLHEP::RandFlat::shoot()*(radiusOuter*radiusOuter-radiusInner*radiusInner) - (radiusInner*radiusInner));
      z = zlength;
    }

    position = G4ThreeVector( radius*cos(phi), radius*sin(phi), z);

  } else if( solid->GetEntityType() == "G4Cons" ) {
    const G4Cons* solidc = static_cast<const G4Cons*>(solid);

    // Select surface cylinder or endcap
    //-  G4cout << "rvolshoot " << rvol << " " <<  theTotalContainer[siz-1] << G4endl;
    unsigned int ia = GetOneArea( solid );

    G4double radius = 0.;
    G4double z = 0.;
    G4double phi;
    G4double zlength = solidc->GetZHalfLength();
    G4double phiStart = solidc->GetStartPhiAngle();
    G4double phiEnd = solidc->GetStartPhiAngle() + solidc->GetDeltaPhiAngle();
    if( phiEnd < 0. || phiEnd > CLHEP::twopi ) phiEnd = fmod(phiEnd+CLHEP::twopi,CLHEP::twopi);
    G4double radiusInnerM = solidc->GetInnerRadiusMinusZ();
    G4double radiusOuterM = solidc->GetOuterRadiusMinusZ();
    G4double radiusInnerP = solidc->GetInnerRadiusPlusZ();
    G4double radiusOuterP = solidc->GetOuterRadiusPlusZ();
    G4double radiusInnerDiff = radiusInnerP - radiusInnerM;
    G4double radiusOuterDiff = radiusOuterP - radiusOuterM;
    G4bool bFullCons = (radiusInnerM == 0. && radiusInnerP && phiStart == 0. && fabs(phiEnd - CLHEP::twopi) < angTolerance );

    for( ;; ){
      phi = CLHEP::RandFlat::shoot()*CLHEP::twopi;
      if(!bFullCons && (phi < phiStart || phi > phiEnd) ){
	continue;
      }
      break;
    }

    if( ia == 0 ) { // cylinder inner
      z = CLHEP::RandFlat::shoot()*2.*zlength- zlength;
      radius = radiusInnerM + (z+zlength)/zlength/2.*radiusInnerDiff;
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) )  G4cout << " cons radius " << radius << " " << radiusInnerM << " diff " <<radiusInnerDiff << " (z+zlength)/zlength/2. " << (z+zlength)/zlength/2. << G4endl;
#endif
    }else if(ia == 1 ) { // cylinder outer
      z = CLHEP::RandFlat::shoot()*2.*zlength- zlength;
      radius = radiusOuterM + (z+zlength)/zlength/2.*radiusOuterDiff;
 #ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) )    G4cout << " cons radius " << radius << " " << radiusOuterM << " diff " <<radiusOuterDiff << " (z+zlength)/zlength/2. " << (z+zlength)/zlength/2. << " sumr " << (z+zlength)/zlength/2.*radiusOuterDiff << G4endl; 
#endif
    } else if(ia == 2 ) { // endcap negative
      G4double radius2 = radiusInnerM*radiusInnerM + CLHEP::RandFlat::shoot()*(radiusOuterM*radiusOuterM-radiusInnerM*radiusInnerM);
      radius = sqrt(radius2);
      z = -zlength;
    } else if(ia == 3 ) { // endcap positive
      G4double radius2 = radiusInnerP*radiusInnerP + CLHEP::RandFlat::shoot()*(radiusOuterP*radiusOuterP-radiusInnerP*radiusInnerP);
      radius = sqrt(radius2);
      z = zlength;
    }

    position = G4ThreeVector( radius*cos(phi), radius*sin(phi), z);

  } else if( solid->GetEntityType() == "G4Box" ) {
    const G4Box* solidc = static_cast<const G4Box*>(solid);
  
    unsigned int ia = GetOneArea( solid );
    G4double xlength = solidc->GetXHalfLength();
    G4double ylength = solidc->GetYHalfLength();
    G4double zlength = solidc->GetZHalfLength();
    G4double x = 0.;
    G4double y = 0.;
    G4double z = 0.;

    // Radius and axial size 
    if( ia == 0 ) { // surface YZ at X negative
      x = -xlength;
      y = -ylength+2*ylength*CLHEP::RandFlat::shoot();
      z = -zlength+2*zlength*CLHEP::RandFlat::shoot();
    } else if( ia == 1 ) { // surface YZ at X positive
      x = xlength;
      y = -ylength+2*ylength*CLHEP::RandFlat::shoot();
      z = -zlength+2*zlength*CLHEP::RandFlat::shoot();
    } else if( ia == 2 ) { // surface XZ at Y negative
      y = -ylength;
      x = -xlength+2*xlength*CLHEP::RandFlat::shoot();
      z = -zlength+2*zlength*CLHEP::RandFlat::shoot();
    } else if( ia == 3 ) { // surface XZ at Y positive
      y = ylength;
      x = -xlength+2*xlength*CLHEP::RandFlat::shoot();
      z = -zlength+2*zlength*CLHEP::RandFlat::shoot();
    } else if( ia == 4 ) { // surface XY at Z negative
      z = -zlength;
      y = -ylength+2*ylength*CLHEP::RandFlat::shoot();
      x = -xlength+2*xlength*CLHEP::RandFlat::shoot();
    } else if( ia == 5 ) { // surface XY at Z positive
      z = zlength;
      y = -ylength+2*ylength*CLHEP::RandFlat::shoot();
      x = -xlength+2*xlength*CLHEP::RandFlat::shoot();
    }
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << " GmPositionSurfacePos ia " << ia << " pos " << G4ThreeVector(x,y,z) << G4endl;
#endif
    position = G4ThreeVector(x,y,z);

  } else {
    G4Exception("GmGenerDistPositionInG4Volumes::GeneratePosInSolid",
		    "Wrong argument",
		    FatalErrorInArgument,
		G4String("Solid type not supported " + solid->GetEntityType() + " , for volume " + solid->GetName() ).c_str());
  }

  return position;
}


//------------------------------------------------------------------------
void GmPositionSurfacePos::BuildSurfaceAreas( const G4VSolid* solid )
{ 
  std::vector<G4double> areas;
  G4double area = 0.;
  if( solid->GetEntityType() == "G4Box" ) {
    const G4Box* solidc = static_cast<const G4Box*>(solid);
    G4double xlength = solidc->GetXHalfLength();
    G4double ylength = solidc->GetYHalfLength();
    G4double zlength = solidc->GetZHalfLength();
    area = ylength*zlength;
    areas.push_back( area );
    area += ylength*zlength;
    areas.push_back( area );
    area += xlength*zlength;
    areas.push_back( area );
    area += xlength*zlength;
    areas.push_back( area );
    area += xlength*ylength;
    areas.push_back( area );
    area += xlength*ylength;
    areas.push_back( area );

  } else if( solid->GetEntityType() == "G4Tubs" ) {
    const G4Tubs* solidc = static_cast<const G4Tubs*>(solid);
    G4double zlength = solidc->GetZHalfLength();
    G4double phiDelta = solidc->GetDeltaPhiAngle();
    G4double radiusInner = solidc->GetInnerRadius();
    G4double radiusOuter = solidc->GetOuterRadius();
    area = phiDelta*radiusInner*zlength;
    areas.push_back( area );
    area += phiDelta*radiusOuter*zlength;
    areas.push_back( area );
    area += phiDelta/2.*(radiusOuter*radiusOuter-radiusInner*radiusInner);
    areas.push_back( area );
    area += phiDelta/2.*(radiusOuter*radiusOuter-radiusInner*radiusInner);
    areas.push_back( area );

  } else if( solid->GetEntityType() == "G4Cons" ) {
    const G4Cons* solidc = static_cast<const G4Cons*>(solid);
    G4double zlength = solidc->GetZHalfLength();
    G4double phiDelta = solidc->GetDeltaPhiAngle();
    G4double radiusInnerM = solidc->GetInnerRadiusMinusZ();
    G4double radiusOuterM = solidc->GetOuterRadiusMinusZ();
    G4double radiusInnerP = solidc->GetInnerRadiusPlusZ();
    G4double radiusOuterP = solidc->GetOuterRadiusPlusZ();
    area = phiDelta*(radiusInnerM+radiusInnerP)/2.*zlength;
    areas.push_back( area );
    area += phiDelta*(radiusOuterM+radiusOuterP)/2.*zlength;
    areas.push_back( area );
    area += phiDelta/2.*(radiusOuterM*radiusOuterM-radiusInnerM*radiusInnerM);
    areas.push_back( area );
    area += phiDelta/2.*(radiusOuterP*radiusOuterP-radiusInnerP*radiusInnerP);
    areas.push_back( area );
  } else if( solid->GetEntityType() == "G4Sphere" ) {
    G4Sphere* solidc = (G4Sphere*)solid;
    // better than repeating a few dozen lines....
    G4Sphere* spherein = new G4Sphere("TEMP", 0., solidc->GetInsideRadius(), solidc->GetStartPhiAngle(), solidc->GetDeltaPhiAngle(), solidc->GetStartThetaAngle(), solidc->GetDeltaThetaAngle() );
    area = spherein->GetSurfaceArea();
    areas.push_back( area );
    G4Sphere* sphereout = new G4Sphere("TEMP", 0., solidc->GetOuterRadius(), solidc->GetStartPhiAngle(), solidc->GetDeltaPhiAngle(), solidc->GetStartThetaAngle(), solidc->GetDeltaThetaAngle() );
    area += spherein->GetSurfaceArea();
    areas.push_back( area );
    delete spherein;
    delete sphereout;
  }

  for( unsigned int ii = 0; ii < areas.size(); ii++ ){
#ifndef GAMOS_NO_VERBOSE
    if( GenerVerb(debugVerb) ) G4cout << " GmPositionSurfacePos::BuildSurfaceAreas() " << ii << " area " << areas[ii] << G4endl;
#endif
  }

  theSurfaceAreas[solid] = areas;

}

//------------------------------------------------------------------------
unsigned int GmPositionSurfacePos::GetOneArea( const G4VSolid* solid )
{ 
  unsigned int ia;
  std::vector<G4double> surfaceAreas = theSurfaceAreas[solid];
  unsigned int siz = surfaceAreas.size();
  G4double rarea = CLHEP::RandFlat::shoot() * surfaceAreas[siz-1];
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) )  G4cout << " GmPositionSurfacePos::GetOneArea() rarea " << rarea << " total area " << surfaceAreas[siz-1] << G4endl;
#endif
  for( ia = 0; ia < siz; ia++ ){
    if( rarea <= surfaceAreas[ia] ) {
      break;
    }
  }

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmPositionSurfacePos::GetOneArea() ia= " << ia << " rarea " << rarea << " total area " << surfaceAreas[siz-1] << G4endl;
#endif

  return ia;
}
