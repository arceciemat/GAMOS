#define _USE_MATH_DEFINES
#include <cmath>
#include "GmGenerDistPositionRectangle.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosGenerator/include/GmParticleSource.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "G4GeometryTolerance.hh"
#include "CLHEP/Random/RandFlat.h"
#include "geomdefs.hh"

//---------------------------------------------------------------------
G4ThreeVector GmGenerDistPositionRectangle::GeneratePosition( const GmParticleSource* )
{
  G4double posX = 0;
  G4double posY = DBL_MAX;
  G4double posZ = 0;

  // Generate (x,y) inside square
  posX = 2.*theWidthX*CLHEP::RandFlat::shoot()-theWidthX;
  posY = 2.*theWidthY*CLHEP::RandFlat::shoot()-theWidthY;
  posZ = 0.;

  G4ThreeVector pos( posX, posY, posZ);

  pos = RotateAndTranslate( pos );

  return pos;

}


//---------------------------------------------------------------------
void GmGenerDistPositionRectangle::SetParams( const std::vector<G4String>& params )
{

  if( params.size() != 2 && params.size() != 5 && params.size() != 8 ) {
    G4Exception(" GmGenerDistPositionRectangle::SetParams",
		"Wrong argument",
		FatalErrorInArgument,
		"To set point you have to add 2, 5 or 8 parameters: WIDTHX WIDTHY  (POS_X POS_Y POS_Z) (DIR_X DIR_Y DIR_Z");
  }
  theWidthX = GmGenUtils::GetValue( params[0] );
  theWidthY = GmGenUtils::GetValue( params[1] );

  if( params.size() >= 5 ) {
    theCentre = G4ThreeVector(GmGenUtils::GetValue( params[2] ), GmGenUtils::GetValue( params[3] ), GmGenUtils::GetValue( params[4] ) );
  }
  if( params.size() >= 8 ) {
    //normalize direction cosines
    G4ThreeVector dir(GmGenUtils::GetValue( params[5] ), GmGenUtils::GetValue( params[6] ), GmGenUtils::GetValue( params[7] ) );
    if( fabs(dir.mag()-1.) > G4GeometryTolerance::GetInstance()->GetSurfaceTolerance() ) {
      G4Exception("GmGenerDistPositionRectangle::SetParams",
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
  if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistPositionRectangle::SetParams widthX " << theWidthX << " wdithY " << theWidthY << " translation " << theCentre << " rotation " << theRotation << G4endl;
#endif

}
