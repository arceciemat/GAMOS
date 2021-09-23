#define _USE_MATH_DEFINES
#include "GmGenerDistDirectionCone2D.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosGenerator/include/GmParticleSource.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "CLHEP/Random/RandFlat.h"
#include "CLHEP/Units/SystemOfUnits.h"

//---------------------------------------------------------------------
GmGenerDistDirectionCone2D::GmGenerDistDirectionCone2D()
{
}

//---------------------------------------------------------------------
G4ThreeVector GmGenerDistDirectionCone2D::GenerateDirection( const GmParticleSource* )
{
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistDirectionCone2D::Generate:  initial dir " << theInitialDir << " opening angleX (deg) " << sqrt(theOpeningAngleX2)/CLHEP::deg << " opening angleY (deg) " << sqrt(theOpeningAngleY2)/CLHEP::deg << G4endl;
#endif

  G4double phi = CLHEP::RandFlat::shoot()*2.*M_PI;

  //---- Get theta angle around theInitialDir
  //---- opening angle depends on phi
  G4double openingAngle = sqrt(1./(std::pow(cos(phi),2)/theOpeningAngleX2+std::pow(sin(phi),2)/theOpeningAngleY2));
  G4double theta = (sqrt(CLHEP::RandFlat::shoot())*openingAngle);
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistDirectionCone2D::Generate:  theta " << theta/CLHEP::deg << " phi " << phi/CLHEP::deg << " openingAngle " << openingAngle/CLHEP::deg  << G4endl;
#endif

  G4ThreeVector newDir = theInitialDir; 
  newDir.rotate( theta, thePerpDir );
#ifndef GAMOS_NO_VERBOSE
   if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistDirectionCone2D::Generate:  dir after theta rotation " << newDir << " theta " << theta/CLHEP::deg << G4endl;
#endif

  //---- Get phi angle around newDir
  newDir.rotate( phi , theInitialDir );


#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistDirectionCone2D::Generate:  final dir " << newDir << " theta= " << newDir.theta()/CLHEP::deg << " phi " << newDir.phi()/CLHEP::deg << G4endl;
#endif

  return newDir;

}

//---------------------------------------------------------------------
void GmGenerDistDirectionCone2D::SetParams( const std::vector<G4String>& params )
{
  if( params.size() != 5 ) {
    G4Exception(" GmGenerDistDirectionCone2D::SetParam",
		"Wrong argument",
		FatalErrorInArgument,
		"To set direction you have to add 5 parameters: DIR_X DIR_Y DIR_Z OPENING_ANGLE_X OPENING_ANGLE_Y");  
  }

  theInitialDir = G4ThreeVector(GmGenUtils::GetValue( params[0] ), GmGenUtils::GetValue( params[1] ), GmGenUtils::GetValue( params[2] ) );

  theOpeningAngleX2 = std::pow(GmGenUtils::GetValue(params[3]),2);
  theOpeningAngleY2 = std::pow(GmGenUtils::GetValue(params[4]),2);

  //----- Get one perpendicular direction
  G4ThreeVector dir(1.,0.,0.);

  if( fabs(fabs(theInitialDir*dir) - 1.) < 1.E-9 ){
    dir = G4ThreeVector(0.,1.,0.);
  }
  thePerpDir = theInitialDir.cross(dir);

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistDirectionCone2D::SetParams theInitialDir " << theInitialDir << "thePerpDir " << thePerpDir << " opening angle X " << sqrt(theOpeningAngleX2)/CLHEP::deg << " Y " << sqrt(theOpeningAngleY2)/CLHEP::deg << G4endl;
#endif


}
