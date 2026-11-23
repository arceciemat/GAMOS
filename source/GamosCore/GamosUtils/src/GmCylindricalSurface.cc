#include "GamosCore/GamosUtils/include/GmCylindricalSurface.hh"
#include "GamosCore/GamosUtils/include/GmLine.hh"
#include "geomdefs.hh"
#include "G4Plane3D.hh"
#include "G4GeometryTolerance.hh"

      // Initialise a single volume, positioned in a frame which is rotated by
      // *pRot and traslated by tlate, relative to the coordinate system of the
      // mother volume pMotherLogical.
      // If pRot=0 the volume is unrotated with respect to its mother.
      // The physical volume is added to the mother's logical volume.
      // Arguments particular to G4PVPlacement:
      //   pMany Currently NOT used. For future use to identify if the volume
      //         is meant to be considered an overlapping structure, or not.
      //   pCopyNo should be set to 0 for the first volume of a given type.
      // This is a very natural way of defining a physical volume, and is
      // especially useful when creating subdetectors: the mother volumes are
      // not placed until a later stage of the assembly program.

//----------------------------------------------------------------------
GmCylindricalSurface::GmCylindricalSurface( const G4float& radius, const G4ThreeVector& trans, const G4RotationMatrix& rotm ): fradius(radius)
{
  ftransform3D = G4Transform3D( rotm.inverse(), -trans );
  Dump( " $$$ creating GmCylindricalSurface ");
}

      // Additional constructor, which expects a G4Transform3D that represents 
      // the direct rotation and translation of the solid (NOT of the frame).  
      // The G4Transform3D argument should be constructed by:
      //  i) First rotating it to align the solid to the system of 
      //     reference of its mother volume *pMotherLogical, and 
      // ii) Then placing the solid at the location Transform3D.getTranslation(),
      //     with respect to the origin of the system of coordinates of the
      //     mother volume.  
      // [ This is useful for the people who prefer to think in terms 
      //   of moving objects in a given reference frame. ]
      // All other arguments are the same as for the previous constructor.

//----------------------------------------------------------------------
GmCylindricalSurface::GmCylindricalSurface( const G4float& radius, const G4Transform3D& trans3D): fradius(radius), ftransform3D(trans3D.inverse())
{
}


//----------------------------------------------------------------------
G4ThreeVector GmCylindricalSurface::Intersect( const GmLine& line ) const
{
  return Intersect( line.GetPoint(), line.GetDir() );

}

//----------------------------------------------------------------------
G4ThreeVector GmCylindricalSurface::Intersect( const G4ThreeVector& pt, const G4ThreeVector& dir ) const
{
  G4ThreeVector localPoint = ftransform3D * G4Point3D(pt);
  G4ThreeVector localDir = ftransform3D * G4Normal3D(dir);
  G4ThreeVector inters = IntersectLocal( localPoint, localDir );
  return  ftransform3D.inverse()*G4Point3D(inters);

}


//----------------------------------------------------------------------
G4ThreeVector GmCylindricalSurface::IntersectLocal( const G4ThreeVector& localPoint, const G4ThreeVector& localDir ) const
{
  if( localDir.perp() == 0. ) {
    G4Exception("GmCylindricalSurface::Intersect",
		"Impossible calculation",
		FatalException,
		"Direction is perpendicular to cylinder axis ");
  }

  float a = pow(localDir.perp(),2);
  float b = 2.*localPoint.x()*localDir.x() + 2.*localPoint.y()*localDir.y();
  float c = pow(localPoint.perp(),2) - pow(fradius,2);
  
  float lam = (-b + sqrt(b*b-4.*a*c) ) / (2.*a);

  //-  G4cout << " lam " << lam << " fradius " <<fradius  << "  pointPerp " << localPoint.perp()  << " dirPerp " << localDir.perp() << G4endl;
  G4ThreeVector inters = localPoint + lam * localDir;

  //-  G4cout << " GmCylindricalSurface::getIntersection " << inters << " " << inters.perp() << G4endl;

  //-  G4cout << " localPoint " << localPoint << " localDir " << localDir << G4endl;
  //  Dump( " cylinder: " );
  return inters;
}


//----------------------------------------------------------------------
G4double GmCylindricalSurface::GetDistanceFromPoint( const G4ThreeVector& pt, const G4ThreeVector& dir ) const
{

  if( dir.mag() == 0. ) {
    G4Exception("GmCylindricalSurface::GetDistanceFromPoint",
		"Impossible calculation",
		FatalException,
		"direction is zero ");

  }

  //transform to local coordinates
  G4ThreeVector localPoint = ftransform3D * G4Point3D(pt) ;
  G4ThreeVector localDir = ftransform3D * G4Normal3D(dir.unit());

  //    G4cout << " global pt " << pt << " dir " << dir << G4endl;
  //    G4cout << " transformed to local coordinates pt " << localPoint << " dir " << localDir << G4endl;
  //    Dump( " cylsurf " );

  // If parallel to cylinder axis there is no intersection
  if( localDir.perp() == 0 ) {
    return kInfinity;
  }


  G4ThreeVector inters = IntersectLocal( localPoint, localDir );
  G4double lam = (inters - localPoint).mag();
  //    G4cout << this << " GmCylindricalSurface::getDistanceFromPoint " << lam << " point " << pt << " direc " << dir << G4endl;
  
  return lam;

}

//----------------------------------------------------------------------
G4double GmCylindricalSurface::GetDistanceFromPoint( const G4ThreeVector& pt ) const
{
  G4ThreeVector localPoint = ftransform3D * G4Point3D(pt);

  return fradius - localPoint.perp();
}


//----------------------------------------------------------------------
G4Plane3D GmCylindricalSurface::GetTangentPlane( const G4ThreeVector& pt ) const
{
  G4ThreeVector localPoint = ftransform3D * G4Point3D(pt);

  //check that point is at cylinder surface
  if( fabs( localPoint.perp() - fradius ) > 1000.*G4GeometryTolerance::GetInstance()->GetSurfaceTolerance() ) {
    G4cerr << " !!WARNING  GmCylindricalSurface::GetTangentPlane: point " << pt << " is not at surface, but it is distant " << localPoint.perp() - fradius << G4endl;
  }

  G4Normal3D normal = localPoint - ftransform3D.getTranslation();
  return G4Plane3D( normal ,pt );

}

//----------------------------------------------------------------------
void GmCylindricalSurface::Dump( G4String msg ) const
{
  G4cout << msg << " radius " << fradius << " centre " <<  ftransform3D.getTranslation() << " rotation " << ftransform3D.getRotation() << G4endl;

}
