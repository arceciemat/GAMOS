#ifndef GmCylindricalSurface_h
#define GmCylindricalSurface_h

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4Plane3D.hh"
class GmLine;

class GmCylindricalSurface 
{
public:
  GmCylindricalSurface( const G4float& radius, const G4ThreeVector& trans=G4ThreeVector(), const G4RotationMatrix& rotm=G4RotationMatrix());
  GmCylindricalSurface( const G4float& radius, const G4Transform3D& trans3D);

  ~GmCylindricalSurface(){};

public:

  G4ThreeVector Intersect( const GmLine& line ) const;
  G4ThreeVector Intersect( const G4ThreeVector& point, const G4ThreeVector& direc ) const;
  G4ThreeVector IntersectLocal( const G4ThreeVector& point, const G4ThreeVector& direc ) const;
  G4double GetDistanceFromPoint( const G4ThreeVector& point, const G4ThreeVector& direc ) const;
  G4double GetDistanceFromPoint( const G4ThreeVector& point ) const;
  G4Plane3D GetTangentPlane( const G4ThreeVector& point ) const;
  void Dump( G4String msg ) const;

 private:
  G4float fradius;
  G4Transform3D ftransform3D;

};

#endif
