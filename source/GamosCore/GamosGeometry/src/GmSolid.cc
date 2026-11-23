#include "GmSolid.hh"

#include <vector>
#include "CLHEP/Vector/ThreeVector.h"
#include "CLHEP/Geometry/Point3D.h"
#include "CLHEP/Vector/Rotation.h"
#include "CLHEP/Geometry/Transform3D.h"
#include "CLHEP/Random/RandFlat.h"

#include "G4VSolid.hh"
#include "G4tgrSolid.hh"
#include "G4tgbGeometryDumper.hh"

//------------------------------------------------------------------------
GmSolid::GmSolid( const G4VSolid* g4solid)
{
  theName = g4solid->GetName();
  theType = g4solid->GetEntityType();
  unsigned int len = theType.length();
  theType = theType.substr(2,len ); 
  for( unsigned int ii = 0; ii < len-2; ii++ ){
    theType[ii] = toupper( theType[ii] );
  }

  G4tgbGeometryDumper* geomDumper = G4tgbGeometryDumper::GetInstance();
  theParams = geomDumper->GetSolidParams( g4solid );

}


//------------------------------------------------------------------------
GmSolid::GmSolid( const G4tgrSolid* tgrsolid)
{
  theName = tgrsolid->GetName();
  theType = tgrsolid->GetType();

  std::vector< std::vector<double>* > vsp = tgrsolid->GetSolidParams();
    //  theParams = (tgrsolid->GetSolidParams())[0]:
  theParams = *(tgrsolid->GetSolidParams()[0]);
}


//------------------------------------------------------------------------
G4Point3D GmSolid::GetRandomPoint() const
{
  G4double x = 0.;
  G4double y = 0.;
  G4double z = 0.;

  if( theType == "TUBS" ){
    // Radius and axial size of NEMA 1994 cylinder (mm)
    G4double innerRadius = theParams[0];
    G4double outerRadius = theParams[1];
    G4double zlength = theParams[2];
    G4double phiStart = theParams[3];
    G4double phiDelta = theParams[4];
    
    unsigned int iii=0;
    //----- Generate (x,y,z) inside cylinder
    //--- Tube with no hole
    if( innerRadius == 0. && phiDelta == 360.*CLHEP::deg ){
      do{
	iii++;
	x = outerRadius*CLHEP::RandFlat::shoot();
	y = outerRadius*CLHEP::RandFlat::shoot();
      } while ((x*x+y*y) > (outerRadius*outerRadius));

      z = zlength*CLHEP::RandFlat::shoot();
    //--- Tube with hole
    } else if( phiDelta == 360.*CLHEP::deg ){
      do {
	iii++;
	x = outerRadius*CLHEP::RandFlat::shoot();
	y = outerRadius*CLHEP::RandFlat::shoot();
    } while ((x*x+y*y) > (outerRadius*outerRadius) || (x*x+y*y) < (innerRadius*innerRadius));
      z = zlength*CLHEP::RandFlat::shoot();
    //--- Tube section with no hole
    }else if( innerRadius == 0. && phiDelta == 360.*CLHEP::deg ){
      z = zlength*CLHEP::RandFlat::shoot();
      x = outerRadius*CLHEP::RandFlat::shoot();
      y = outerRadius*CLHEP::RandFlat::shoot();
      G4ThreeVector vec(x,y,z);
      G4double vecphi = vec.phi();
      do {
	iii++;
	x = outerRadius*CLHEP::RandFlat::shoot();
	y = outerRadius*CLHEP::RandFlat::shoot();
      } while ((x*x+y*y) > (outerRadius*outerRadius) || (x*x+y*y) < (innerRadius*innerRadius) || vecphi < phiStart || vecphi > phiStart+phiDelta );

    //--- Tube section with hole
    } else {
      z = zlength*CLHEP::RandFlat::shoot();
      x = outerRadius*CLHEP::RandFlat::shoot();
      y = outerRadius*CLHEP::RandFlat::shoot();
      G4ThreeVector vec(x,y,z);
      G4double vecphi = vec.phi();
      do {
	iii++;
	x = outerRadius*CLHEP::RandFlat::shoot();
	y = outerRadius*CLHEP::RandFlat::shoot();
      } while ((x*x+y*y) > (outerRadius*outerRadius) || vecphi < phiStart || vecphi > phiStart+phiDelta );
    }
  } else if( theType == "SPHERE" ){
    G4double innerRadius = theParams[0];
    G4double outerRadius = theParams[1];
    if( theParams[3] != 360.*CLHEP::deg || theParams[5] != 180.*CLHEP::deg ){
G4cerr << " param " << theParams[3] << " " << theParams[5] << G4endl;
 G4Exception(" GmSolid::GetRandomPoint",
	     "Wrong argument",
	     FatalErrorInArgument,
	     "Sphere with no 360 phi or theta is not supported yet " );
    }
    
    unsigned int iii=0;
    //----- Generate (x,y,z) inside sphere
    //--- 360o sphere
    //    if( innerRadius == 0. && phiDelta == 360.*deg ){
    do {
      iii++;
      x = outerRadius*CLHEP::RandFlat::shoot();
      y = outerRadius*CLHEP::RandFlat::shoot();
      z = outerRadius*CLHEP::RandFlat::shoot();
    } while ((x*x+y*y+z*z) < (innerRadius*innerRadius) || (x*x+y*y+z*z) > (outerRadius*outerRadius) );

  } else if( theType == "ORB" ){
    G4double radius = theParams[0];
    
    unsigned int iii=0;
    //----- Generate (x,y,z) inside orb
    do {
      iii++;
      x = radius*CLHEP::RandFlat::shoot();
      y = radius*CLHEP::RandFlat::shoot();
      z = radius*CLHEP::RandFlat::shoot();
    } while ((x*x+y*y+z*z) > (radius*radius) );
    
  } else {
    G4Exception(" GmSolid::GetRandomPoint",
		"Wrong argument",
		FatalErrorInArgument,
		G4String("Solid type not supported yet " + theType ).c_str());
  }
  
#ifndef GAMOS_NO_VERBOSE
  //  if( GenerVerb(infoVerb) ) G4cout << " GmSolid::GetRandomPoint::Generate pos " << G4Point3D(x,y,z) << " " << iii << G4endl;
#endif
  return G4Point3D( x, y, z);
}


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
GmSolid::~GmSolid()
{
}

/*
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
std::ostream& operator<<(std::ostream& os, const GmSolid& sol) 
{
  os << "GmSolid";

  return os;
}
*/
