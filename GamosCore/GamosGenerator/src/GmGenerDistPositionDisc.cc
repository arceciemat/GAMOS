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
  if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistPositionDisc::Generate pos before rotation " << pos << G4endl;
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
    if( fabs(dir.mag()-1.) > G4GeometryTolerance::GetInstance()->GetSurfaceTolerance() ) {
      G4Exception("GmGenerDistPositionDisc::SetParams",
		  "Warning",
		  JustWarning,
		  G4String("direction cosines are normalized to one, they were " + GmGenUtils::ftoa(dir.mag())).c_str());
      dir /= dir.mag();
    } 
    G4double angx = -asin(dir.y());
    // there are always two solutions angx, angy and PI-angx, PI+angy, choose first
    G4double angy;
    if( dir.y() == 1. ) {
      angy = 0.;
    } else if( dir.y() == 0. ) {
      angy = 0.;
    } else {
      angy = asin( dir.x()/sqrt(1-dir.y()*dir.y()) );
    }

    // choose between  angy and PI-angy
    if( dir.z() * cos(angx)*cos(angy) < 0 ) angy = M_PI - angy;
    theRotation.rotateX( angx );
    theRotation.rotateY( angy );
  }
  
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistPositionDisc::SetParams radius " << theRadius << " translation " << theCentre << " rotation " << theRotation << G4endl;
#endif

}
