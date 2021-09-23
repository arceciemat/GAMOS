#define _USE_MATH_DEFINES
#include <cmath>
#include "GmGenerDistDirectionCone2DGaussian.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosGenerator/include/GmParticleSource.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "CLHEP/Random/RandFlat.h"
#include "CLHEP/Units/SystemOfUnits.h"

//---------------------------------------------------------------------
GmGenerDistDirectionCone2DGaussian::GmGenerDistDirectionCone2DGaussian()
{
  theSigmaOpeningAngleX2 = -1.e-9;
  theSigmaOpeningAngleY2 = -1.e-9;
}

//---------------------------------------------------------------------
G4ThreeVector GmGenerDistDirectionCone2DGaussian::GenerateDirection( const GmParticleSource* )
{
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistDirectionCone2DGaussian::Generate:  initial dir " << theInitialDir << " opening angleX (deg) " << sqrt(theSigmaOpeningAngleX2)/CLHEP::deg << " opening angleY (deg) " << sqrt(theSigmaOpeningAngleY2)/CLHEP::deg << G4endl;
#endif
  G4double phi = CLHEP::RandFlat::shoot()*2.*M_PI;
  G4double sigmaOpeningAngle = sqrt(1./(std::pow(cos(phi),2)/theSigmaOpeningAngleX2+std::pow(sin(phi),2)/theSigmaOpeningAngleY2));
  G4double sigmaOpeningAngleC = -2.*sigmaOpeningAngle*sigmaOpeningAngle;
  G4double theta = sqrt(sigmaOpeningAngleC*log(1-CLHEP::RandFlat::shoot()) );
  //  G4cout << " GmGenerDistDirectionCone2DGaussian " << theta << " phi " << phi << " sigmaOpeningAngC " << sigmaOpeningAngleC << G4endl; //GDEB
  
  /*  //---- Get theta angle around theInitialDir
  G4double theta = sqrt(theSigmaOpeningAngleXM*log(1-CLHEP::RandFlat::shoot()) );
  //  G4double theta = acos(val);

  //---- Get phi angle around newDir
  G4double phi = CLHEP::RandFlat::shoot()*2.*M_PI;
  phi = 0./2.;
  //--- recalculate theta, so that it is an ellipse
  G4double radXY = sin(theta); // put is as given by theta, but as phi!=0, it has to be put in the ellipse
  G4double rtheta = radXY*sqrt(1./( cos(phi)*cos(phi)+sin(phi)*sin(phi)*theSigmaOpeningAngleXY2 ));
  G4cout << " GmGenerDistDirectionCone2DGaussian " << theta << " phi " << phi << " radXY " << radXY << " rtheta " << rtheta << " : " << asin(rtheta) << " DIV " << theta/asin(rtheta) << G4endl; //GDEB
  theta = asin(rtheta);
  */
  
  G4ThreeVector newDir = theInitialDir; 
  newDir.rotate( theta, thePerpDir );
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistDirectionCone2DGaussian::Generate:  dir after theta rotation " << newDir << " theta " << theta << G4endl;
#endif
  newDir.rotate( phi , theInitialDir );

#ifndef GAMOS_NO_VERBOSE
   if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistDirectionCone2DGaussian::Generate:  final dir " << newDir << G4endl;
#endif

  return newDir;

}

//---------------------------------------------------------------------
void GmGenerDistDirectionCone2DGaussian::SetParams( const std::vector<G4String>& params )
{
  if( params.size() != 5 ) {
    G4Exception(" GmGenerDistDirectionCone2DGaussian::SetParam",
		"Wrong argument",
		FatalErrorInArgument,
		"To set direction you have to add 5 parameters: DIR_X DIR_Y DIR_Z SIGMA_OPENING_ANGLEX SIGMA_OPENING_ANGLEY");
  }

  theInitialDir = G4ThreeVector(GmGenUtils::GetValue( params[0] ), GmGenUtils::GetValue( params[1] ), GmGenUtils::GetValue( params[2] ) );

  G4double sigma = GmGenUtils::GetValue( params[3] );
  theSigmaOpeningAngleX2 = sigma*sigma;
  sigma *= sqrt(2.);  
  theSigmaOpeningAngleXM = -2.*sigma*sigma; // sigma that will be sampled
  sigma = GmGenUtils::GetValue( params[4] );
  theSigmaOpeningAngleY2 = sigma*sigma;
  theSigmaOpeningAngleXY2 = theSigmaOpeningAngleX2/theSigmaOpeningAngleY2;

  //----- Get one perpendicular direction
  G4ThreeVector dir(1.,0.,0.);

  if( fabs(fabs(theInitialDir*dir) - 1.) < 1.E-9 ){
    dir = G4ThreeVector(0.,1.,0.);
  }
  thePerpDir = theInitialDir.cross(dir);

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistDirectionCone2DGaussian::SetParams theInitialDir " << theInitialDir << "thePerpDir " << thePerpDir << " opening angle X " << sqrt(theSigmaOpeningAngleX2) << " angle Y " << sqrt(theSigmaOpeningAngleY2) << G4endl;
#endif


}
