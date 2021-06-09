#include "GamosCore/GamosUtils/include/GmCone.hh"
#include "GamosCore/GamosUtils/include/GmLine.hh"
#include <math.h>
#include <iomanip>
#include "G4RotationMatrix.hh"
#include "G4tgbRotationMatrix.hh"

//--------------------------------------------------------------------
GmCone::GmCone(const G4ThreeVector& pnt, const G4ThreeVector& dir, const G4double angle ): thePoint(pnt), theAngle( angle )
{
  theDir = dir.unit();
  //  G4cout << " Created GmCone pnt " << thePoint << " dir " << theDir << " angle " << theAngle << G4endl;
}

//------------------- -------------------------------------------------
G4double GmCone::GetDist( const G4ThreeVector& pt ) const
{
  GmLine axis( thePoint, theDir );
  G4ThreeVector pClosestInAxis = axis.GetDistPoint( pt );
  G4int sign = 1;
  if( theDir * (pt-thePoint)/(pt-thePoint).mag() < 0. ) sign = -1;
  G4ThreeVector pNormalInAxis = pClosestInAxis + sign * theDir * (pClosestInAxis-pt).mag() * tan( theAngle );
  GmLine normal( pt, pNormalInAxis-pt );
 
  std::vector<G4ThreeVector> intersPair = Intersect( normal );
  //  G4cout << " GmCone::GetDist pt " << pt << " pClosestInAxis " << pClosestInAxis <<  " pNormalInAxis " << pNormalInAxis << "  ninters " << intersPair.size() << G4endl;

  //  G4cout << " GmCone::GetDist " << dist << " point " << thePoint << " dir " << sign*theDir << " angle " << theAngle/deg << " pt " << pt << G4endl;

  G4double dist = 0.;
  if(intersPair.size() == 1 ) {
    dist = (intersPair[0] - pt).mag();
  } else if( intersPair.size() == 2 ) {
    dist = ((intersPair[0]-pt).mag() < (intersPair[1]-pt).mag() ) ? (intersPair[0] - pt).mag() : (intersPair[1] - pt).mag();
  } else {
    dist = DBL_MAX;
    G4Exception("GmCone::GetDist",
		" no intersection normal - cone ",
		JustWarning,
		"");
  }

  return dist ;

  /*
  G4ThreeVector vect = pt - thePoint;
  vect *= 1./vect.mag();

  double ca2 = pow(cos(theAngle),2.);
  double a = 1.- ca2;
  double b = 2. * theDir*vect * (1. - ca2);
  double c = pow(theDir*vect,2.) - ca2;
  double lam = (-b + sqrt(b*b-4*a*c)) / (2.*a);

  GmLine line( thePoint, theDir+vect*lam);

  return line.GetDist( pt );
  */
}


//--------------------------------------------------------------------
std::vector<G4ThreeVector> GmCone::Intersect( const GmLine& line ) const
{
  //  G4cout << "GmCone::Intersect cone " << *this << G4endl << " line " << line << G4endl;

  std::vector<G4ThreeVector> inters;
 
  G4ThreeVector Pc1 = thePoint;
  G4ThreeVector Pc2 = thePoint-theDir;
  G4ThreeVector Pp1 = line.GetPoint();
  G4ThreeVector Pp2 = line.GetPoint() + line.GetDir();

  G4ThreeVector v1 = Pc1 - Pp1;
  G4ThreeVector v2 = Pc2 - Pc1;
  G4ThreeVector v3 = Pp2 - Pp1;
  // G4cout << "GmCone::Intersect v1 " << v1 << " v2 " << v2 << " v3 " << v3 << G4endl;
 
  double v12=v1*v2;
  double v32=v3*v2;
  double v11=v1.mag2();
  double v13=v1*v3;
  double v33=v3.mag2();
  double v22=v2.mag2();
  double cos_w = cos( theAngle );
  double c2w=cos_w*cos_w;
  
  double a= v32*v32-v33*v22*c2w;
  double b= 2.0*(-v12*v32+v13*v22*c2w);
  double c= v12*v12-v11*v22*c2w;
  //  G4cout << "GmCone::Intersect a " << a << " " << v32*v32 << " - " << v33*v22*c2w << " v32 " << v32 << " v33 " << v33 << " v22 " << v22 << " c2w " << c2w << G4endl;

  //  G4cout << "GmCone::Intersect a " << a << " b " << b << " c " << c << G4endl;

  double lmd1,lmd2;
  double t2=b*b-4*a*c;
  if (t2 < 0){
    printf("--> NO REAL SOLUTION FOR THE INTERSECTION!! \n");
  }
  else{
    lmd1=(-b+sqrt(t2))/(2*a);
    lmd2=(-b-sqrt(t2))/(2*a);

    //G4cout << "GmCone::Intersect lmd1 " << lmd1 << " lmd2 " << lmd2 << G4endl;
    //G4cout << "GmCone::Intersect (-b-sqrt(t2)) " << (-b-sqrt(t2)) << G4endl;
    inters.push_back( Pp1 + lmd1*(Pp2-Pp1) );
    inters.push_back( Pp1 + lmd2*(Pp2-Pp1) );
    
  }

 
  return inters;
}

//--------------------------------------------------------------------
std::ostream& operator<<(std::ostream& os, const GmCone& con)
{
  os << " GmLine: Point= " << con.thePoint << "  Dir= " << con.theDir << "  Angle= " << con.theAngle;
  return os;

}

G4RotationMatrix* GmCone::BuildG4RotMatrixFrom9( const G4ThreeVector& colx, const G4ThreeVector& coly, const G4ThreeVector& colz ) const 
{
  // Now create a G4RotationMatrix (HepRotation), which can be left handed. 
  // This is not foreseen in CLHEP, but can be achieved using the
  // constructor which does not check its input arguments!

  CLHEP::HepRep3x3 rottemp(colx.x(),coly.x(),colz.x(),  // matrix representation
                           colx.y(),coly.y(),colz.y(),  // (inverted)
                           colx.z(),coly.z(),colz.z());
  
  G4RotationMatrix* rotMat = new G4RotationMatrix(rottemp);

  return rotMat;
}
