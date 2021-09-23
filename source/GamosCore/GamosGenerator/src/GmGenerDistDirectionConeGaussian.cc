#define _USE_MATH_DEFINES
#include <cmath>
#include "GmGenerDistDirectionConeGaussian.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosGenerator/include/GmParticleSource.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "CLHEP/Random/RandFlat.h"
#include "CLHEP/Units/SystemOfUnits.h"

//---------------------------------------------------------------------
GmGenerDistDirectionConeGaussian::GmGenerDistDirectionConeGaussian()
{
  theSigmaOpeningAngleC = -1.e-9;
}

//---------------------------------------------------------------------
G4ThreeVector GmGenerDistDirectionConeGaussian::GenerateDirection( const GmParticleSource* )
{
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistDirectionConeGaussian::Generate:  initial dir " << theInitialDir << " opening angle (deg) " << theSigmaOpeningAngleC/CLHEP::deg << G4endl;
#endif
  //---- Get theta angle around theInitialDir
  G4double rnd = CLHEP::RandFlat::shoot(); 
  G4double theta = sqrt(theSigmaOpeningAngleC*log(1-rnd) );
  //  G4cout << " CONE " << theta << " " << rnd << " " << log(1-rnd) << " " << theSigmaOpeningAngleC*log(1-rnd) << std::endl; //GDEB
  //  G4double theta = sqrt(theSigmaOpeningAngleC*log(1-CLHEP::RandFlat::shoot()) );
  //  G4double theta = acos(val);

  G4ThreeVector newDir = theInitialDir; 
  newDir.rotate( theta, thePerpDir );
#ifndef GAMOS_NO_VERBOSE
   if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistDirectionConeGaussian::Generate:  dir after theta rotation " << newDir << " theta " << theta << G4endl;
#endif

  //---- Get phi angle around newDir
  G4double phi = CLHEP::RandFlat::shoot()*2.*M_PI;
  newDir.rotate( phi , theInitialDir );

#ifndef GAMOS_NO_VERBOSE
   if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistDirectionConeGaussian::Generate:  final dir " << newDir << G4endl;
#endif

  return newDir;

}

//---------------------------------------------------------------------
void GmGenerDistDirectionConeGaussian::SetParams( const std::vector<G4String>& params )
{
  if( params.size() != 4 ) {
    G4Exception(" GmGenerDistDirectionConeGaussian::SetParam",
		"Wrong argument",
		FatalErrorInArgument,
		"To set direction you have to add 4 parameters: DIR_X DIR_Y DIR_Z SIGMA_OPENING_ANGLE");  
  }

  theInitialDir = G4ThreeVector(GmGenUtils::GetValue( params[0] ), GmGenUtils::GetValue( params[1] ), GmGenUtils::GetValue( params[2] ) );

  G4double sigma = GmGenUtils::GetValue( params[3] )*sqrt(2.);  
  theSigmaOpeningAngleC = -2.*sigma*sigma;

  //----- Get one perpendicular direction
  G4ThreeVector dir(1.,0.,0.);

  if( fabs(fabs(theInitialDir*dir) - 1.) < 1.E-9 ){
    dir = G4ThreeVector(0.,1.,0.);
  }
  thePerpDir = theInitialDir.cross(dir);

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistDirectionConeGaussian::SetParams theInitialDir " << theInitialDir << "thePerpDir " << thePerpDir << " opening angle " << theSigmaOpeningAngleC << G4endl;
#endif


}
