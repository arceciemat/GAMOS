#include "GmPlane.hh"

#include "G4Point3D.hh"
#include "G4ThreeVector.hh"

//---------------------------------------------------------------------
GmPlane::GmPlane(G4double a, G4double b, G4double c, G4double d)
  : G4Plane3D( a, b, c, d ) 
{
}

//---------------------------------------------------------------------
GmPlane::GmPlane(const G4Normal3D &n, const G4Point3D &p)
  : G4Plane3D( n, p ) 
{
}

//---------------------------------------------------------------------
GmPlane::GmPlane(const G4Point3D &p1,
                          const G4Point3D &p2,
                          const G4Point3D &p3)
  : G4Plane3D( p1, p2, p3 )
{
}

//---------------------------------------------------------------------
GmPlane::~GmPlane()
{
}

//---------------------------------------------------------------------
G4ThreeVector GmPlane::Intersect( const G4ThreeVector& pt, const G4ThreeVector& dir ) const
{
  G4double lam = GetDistanceFromPoint( pt, dir );
  G4Point3D inters = pt + lam * dir;

  return inters;
}

//---------------------------------------------------------------------
G4double GmPlane::GetDistanceFromPoint( const G4ThreeVector& pt, const G4ThreeVector& dir ) const
{
  if( std::fabs( dir.mag() -1. ) > 1.E-6 )
  {
    G4cerr << "WARNING - GmPlane::GetDistanceFromPoint()"
           << G4endl
           << "          Direction is not a unit vector: " << dir << " !"
           << G4endl;
  }
  G4double dist = -(a_ * pt.x() + b_ * pt.y() + c_ * pt.z() + d_)
                 / (a_ * dir.x() + b_ * dir.y() + c_ * dir.z() );

  return dist;
}

//---------------------------------------------------------------------
G4double GmPlane::GetDistanceFromPoint( const G4ThreeVector& pt ) const
{
  G4ThreeVector vec = point() - pt;
  G4double alpha = std::acos( vec * normal() / vec.mag() / normal().mag() );
  G4double dist = std::fabs(vec.mag() * std::cos( alpha ));
  
  return dist;
}

//---------------------------------------------------------------------
void GmPlane::Dump( const G4String& msg ) const
{
  G4cout << msg << " point = " << point()
                << " normal = " << normal() << G4endl;
}
