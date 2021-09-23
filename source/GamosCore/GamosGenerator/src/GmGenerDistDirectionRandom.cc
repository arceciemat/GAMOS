#define _USE_MATH_DEFINES
#include <cmath>
#include "GmGenerDistDirectionRandom.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosGenerator/include/GmParticleSource.hh"
#include "CLHEP/Random/RandFlat.h"
#include "CLHEP/Units/SystemOfUnits.h"

G4ThreeVector GmGenerDistDirectionRandom::GenerateDirection( const GmParticleSource* )
{
  //--- supplied direction is not used
  double costheta = 2*CLHEP::RandFlat::shoot()-1;
  double sintheta = sqrt(1.-costheta*costheta);
  double phi = 2*CLHEP::pi*CLHEP::RandFlat::shoot();

  G4ThreeVector dir = G4ThreeVector( sintheta*cos(phi), sintheta*sin(phi), costheta );
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistDirectionRandom::Generate  dir "  << dir << G4endl;
#endif
  return dir;

}
