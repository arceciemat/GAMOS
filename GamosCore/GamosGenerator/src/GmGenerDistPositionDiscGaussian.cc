#define _USE_MATH_DEFINES
#include <cmath>
#include "GmGenerDistPositionDiscGaussian.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosGenerator/include/GmParticleSource.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "CLHEP/Random/RandFlat.h"
#include "CLHEP/Random/RandGauss.h"
#include "G4GeometryTolerance.hh"
#include "geomdefs.hh"

//---------------------------------------------------------------------
G4ThreeVector GmGenerDistPositionDiscGaussian::GeneratePosition( const GmParticleSource* )
{
  G4double val = sqrt(theSigmaM*log(1-CLHEP::RandFlat::shoot()) );
  G4double phi = 360.*CLHEP::deg*CLHEP::RandFlat::shoot();

  G4ThreeVector pos( val*cos(phi), val*sin(phi), 0.);
  //--- Place it and rotate it
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistPositionDiscGaussian::Generate pos before rotation " << pos << G4endl;
#endif
  pos = theRotation * pos;
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistPositionDiscGaussian::Generate pos before traslation " << pos << G4endl;
#endif
  pos += theCentre;

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistPositionDiscGaussian::Generate pos " << pos << G4endl;
#endif
  return pos;

}


//---------------------------------------------------------------------
void GmGenerDistPositionDiscGaussian::SetParams( const std::vector<G4String>& params )
{

  if( params.size() != 1 && params.size() != 4 && params.size() != 7 ) {
    G4Exception(" GmGenerDistPositionDiscGaussian::SetParams",
		"Wrong argument",
		FatalErrorInArgument,
		"To set point you have to add 1, 4 or 7 parameters: SIGMA (POS_X POS_Y POS_Z) (DIR_X DIR_Y DIR_Z");
  }
  if( params.size() >= 1 ) {
    G4double sigma = GmGenUtils::GetValue( params[0] )*sqrt(2.);
    theSigmaM = -2.*sigma*sigma;
  }
  if( params.size() >= 4 ) {
    theCentre = G4ThreeVector(GmGenUtils::GetValue( params[1] ), GmGenUtils::GetValue( params[2] ), GmGenUtils::GetValue( params[3] ) );
  }
  if( params.size() >= 7 ) {
    //normalize direction cosines
    G4ThreeVector dir(GmGenUtils::GetValue( params[4] ), GmGenUtils::GetValue( params[5] ), GmGenUtils::GetValue( params[6] ) );
    if( fabs(dir.mag()-1.) > G4GeometryTolerance::GetInstance()->GetSurfaceTolerance() ) {
      G4Exception("GmGenerDistPositionDiscGaussian::SetParams",
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

}
