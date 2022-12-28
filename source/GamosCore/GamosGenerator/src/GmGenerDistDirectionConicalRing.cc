#define _USE_MATH_DEFINES
#include <cmath>
#include "GmGenerDistDirectionConicalRing.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosGenerator/include/GmParticleSource.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "CLHEP/Random/RandFlat.h"
#include "CLHEP/Units/SystemOfUnits.h"

//---------------------------------------------------------------------
GmGenerDistDirectionConicalRing::GmGenerDistDirectionConicalRing()
{
}

//---------------------------------------------------------------------
G4ThreeVector GmGenerDistDirectionConicalRing::GenerateDirection( GmParticleSource* )
{
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistDirectionConicalRing::Generate:  initial dir " << theAxisDir << " initial angle (deg) " << theInitialAngle/CLHEP::deg << G4endl;
#endif
  //---- Get theta angle around theAxisDir
  G4double theta = theInitialAngle+(sqrt(CLHEP::RandFlat::shoot())*theCoveredAngle);

  G4ThreeVector newDir = theAxisDir; 
  newDir.rotate( theta, thePerpDir );
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistDirectionConicalRing::Generate:  dir after theta rotation " << newDir << " theta " << theta/CLHEP::deg << " " << thePerpDir << G4endl;
#endif

  //---- Get phi angle around newDir
  G4double phi = CLHEP::RandFlat::shoot()*2.*M_PI;
  newDir.rotate( phi , theAxisDir );

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistDirectionConicalRing::Generate:  final dir " << newDir << " phi " << phi/CLHEP::deg << G4endl;
#endif

  return newDir;

}

//---------------------------------------------------------------------
void GmGenerDistDirectionConicalRing::SetParams( const std::vector<G4String>& params )
{
  if( params.size() != 5 ) {
    G4Exception(" GmGenerDistDirectionConicalRing::SetParam",
		"Wrong argument",
		FatalErrorInArgument,
		"To set direction you have to add 4 parameters: DIR_X DIR_Y DIR_Z INITAL_ANGLE OPENING_ANGLE");  
  }

  theAxisDir = G4ThreeVector(GmGenUtils::GetValue( params[0] ), GmGenUtils::GetValue( params[1] ), GmGenUtils::GetValue( params[2] ) );

  theInitialAngle = GmGenUtils::GetValue(params[3]);
  theOpeningAngle = GmGenUtils::GetValue(params[4]);
  theCoveredAngle = theOpeningAngle - theInitialAngle;

  //----- Get one perpendicular direction
  G4ThreeVector dir(1.,0.,0.);
  if( fabs(fabs(theAxisDir*dir) - 1.) < 1.E-9 ){
    dir = G4ThreeVector(0.,1.,0.);
  }
  thePerpDir = theAxisDir.cross(dir);

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistDirectionConicalRing::SetParams theAxisDir " << theAxisDir << "thePerpDir " << thePerpDir << " opening angle " << theOpeningAngle << G4endl;
#endif


}

//---------------------------------------------------------------------
void GmGenerDistDirectionConicalRing::SetDirection( G4ThreeVector dirIni )
{
  theAxisDir = dirIni;
  G4ThreeVector dirPP(1.,0.,0.);
  if( fabs(fabs(theAxisDir*dirPP) - 1.) < 1.E-9 ){
    dirPP = G4ThreeVector(0.,1.,0.);
  }
  thePerpDir = theAxisDir.cross(dirPP);
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistDirectionConicalRing::SetDirection theAxisDir " << theAxisDir << "thePerpDir " << thePerpDir << G4endl;
#endif
}
