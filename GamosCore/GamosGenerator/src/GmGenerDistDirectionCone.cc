#define _USE_MATH_DEFINES
#include <cmath>
#include "GmGenerDistDirectionCone.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosGenerator/include/GmParticleSource.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "CLHEP/Random/RandFlat.h"
#include "CLHEP/Units/SystemOfUnits.h"

//---------------------------------------------------------------------
GmGenerDistDirectionCone::GmGenerDistDirectionCone()
{
}

//---------------------------------------------------------------------
G4ThreeVector GmGenerDistDirectionCone::GenerateDirection( const GmParticleSource* )
{
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistDirectionCone::Generate:  initial dir " << theInitialDir << " opening angle (deg) " << theOpeningAngle/CLHEP::deg << G4endl;
#endif
  //---- Get theta angle around theInitialDir
  G4double theta = (sqrt(CLHEP::RandFlat::shoot())*theOpeningAngle);

  G4ThreeVector newDir = theInitialDir; 
  newDir.rotate( theta, thePerpDir );
#ifndef GAMOS_NO_VERBOSE
   if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistDirectionCone::Generate:  dir after theta rotation " << newDir << " theta " << theta << G4endl;
#endif

  //---- Get phi angle around newDir
  G4double phi = CLHEP::RandFlat::shoot()*2.*M_PI;
  newDir.rotate( phi , theInitialDir );

#ifndef GAMOS_NO_VERBOSE
   if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistDirectionCone::Generate:  final dir " << newDir << G4endl;
#endif

  return newDir;

}

//---------------------------------------------------------------------
void GmGenerDistDirectionCone::SetParams( const std::vector<G4String>& params )
{
  if( params.size() != 4 ) {
    G4Exception(" GmGenerDistDirectionCone::SetParam",
		"Wrong argument",
		FatalErrorInArgument,
		"To set direction you have to add 3 parameters: DIR_X DIR_Y DIR_Z OPENING_ANGLE");  
  }

  theInitialDir = G4ThreeVector(GmGenUtils::GetValue( params[0] ), GmGenUtils::GetValue( params[1] ), GmGenUtils::GetValue( params[2] ) );

  theOpeningAngle = GmGenUtils::GetValue(params[3]);

  //----- Get one perpendicular direction
  G4ThreeVector dir(1.,0.,0.);

  if( fabs(fabs(theInitialDir*dir) - 1.) < 1.E-9 ){
    dir = G4ThreeVector(0.,1.,0.);
  }
  thePerpDir = theInitialDir.cross(dir);

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistDirectionCone::SetParams theInitialDir " << theInitialDir << "thePerpDir " << thePerpDir << " opening angle " << theOpeningAngle << G4endl;
#endif


}
