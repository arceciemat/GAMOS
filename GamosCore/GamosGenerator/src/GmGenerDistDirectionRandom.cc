#define _USE_MATH_DEFINES
#include <cmath>
#include "GmGenerDistDirectionRandom.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosGenerator/include/GmParticleSource.hh"
#include "CLHEP/Random/RandFlat.h"

G4ThreeVector GmGenerDistDirectionRandom::GenerateDirection( const GmParticleSource* )
{
  //--- supplied direction is not used
  double theta;
  for( ;; ){
    theta = M_PI*CLHEP::RandFlat::shoot();
    double tt = CLHEP::RandFlat::shoot();
    if( tt < sin(theta) ) break;
  }
  double phi = 2*M_PI*CLHEP::RandFlat::shoot();

  G4ThreeVector dir = G4ThreeVector( sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta) );

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistDirectionRandom::Generate  dir "  << dir << G4endl;
#endif
  return dir;

}
