#define _USE_MATH_DEFINES
#include <cmath>

#include "GmGenerDistPositionDisc.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosGenerator/include/GmParticleSource.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "G4GeometryTolerance.hh"
#include "CLHEP/Random/RandFlat.h"
#include "geomdefs.hh"

//---------------------------------------------------------------------
GmGenerDistPositionDisc::GmGenerDistPositionDisc()
{
  theRadius = 0.;
  theCentre = G4ThreeVector( 0., 0., 0. );
  theRotation = G4RotationMatrix();
}

//---------------------------------------------------------------------
G4ThreeVector GmGenerDistPositionDisc::GeneratePosition( const GmParticleSource* )
{
  int iii=0;
  G4double x = 0;
  G4double y = DBL_MAX;
  G4double z = 0;

  // Generate (x,y) inside cylinder
  while ((x*x+y*y) > (theRadius*theRadius)){ 
    iii++;
    x = theRadius*(-1+2*CLHEP::RandFlat::shoot());
    y = theRadius*(-1+2*CLHEP::RandFlat::shoot());
    //    G4cout << " x y " << x << " " << y << " radius " << theRadius << G4endl;
  }
  z = 0.;

  G4ThreeVector pos( x, y, z);
  /*  G4ThreeVector pos( 0., 0., 10.);
  G4RotationMatrix rotm;
  rotm.rotateX(30.*deg);
  //  rotm.rotateY(30.*deg);
  pos = rotm * pos;

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistPositionDisc::Generate pos before rotation " << pos << G4endl;
#endif
  */
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << this <<" GmGenerDistPositionDisc::Generate pos before rotation " << pos << " radius " << theRadius << G4endl;
#endif
  pos = theRotation * pos;
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistPositionDisc::Generate pos before translation " << pos << G4endl;
#endif
  pos += theCentre;
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistPositionDisc::Generate pos " << pos << G4endl;
#endif

  return pos;

}


//---------------------------------------------------------------------
void GmGenerDistPositionDisc::SetParams( const std::vector<G4String>& params )
{
  theRotation = G4RotationMatrix();
  if( params.size() != 1 && params.size() != 4 && params.size() != 7 ) {
    G4Exception(" GmGenerDistPositionDisc::SetParams",
		"Wrong argument",
		FatalErrorInArgument,
		"To set point you have to add 1, 4 or 7 parameters: RADIUS (POS_X POS_Y POS_Z) (DIR_X DIR_Y DIR_Z");
  }
  theRadius = GmGenUtils::GetValue( params[0] );
  
  if( params.size() >= 4 ) {
    theCentre = G4ThreeVector(GmGenUtils::GetValue( params[1] ), GmGenUtils::GetValue( params[2] ), GmGenUtils::GetValue( params[3] ) );
  }
  if( params.size() >= 7 ) {
    //normalize direction cosines
    G4ThreeVector dir(GmGenUtils::GetValue( params[4] ), GmGenUtils::GetValue( params[5] ), GmGenUtils::GetValue( params[6] ) );
    SetRotation( dir );

  }    

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistPositionDisc::SetParams radius " << theRadius << " translation " << theCentre << " rotation " << theRotation << G4endl;
#endif

}

