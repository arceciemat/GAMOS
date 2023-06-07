#define _USE_MATH_DEFINES
#include <cmath>
#include "GmGenerDistPosition2DDoubleGaussian.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosGenerator/include/GmParticleSource.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "CLHEP/Random/RandFlat.h"
#include "CLHEP/Random/RandGauss.h"
#include "G4GeometryTolerance.hh"
#include "geomdefs.hh"

//---------------------------------------------------------------------
GmGenerDistPosition2DDoubleGaussian::GmGenerDistPosition2DDoubleGaussian()
{
  theCentre = G4ThreeVector(0., 0., 0.);
  theRotation = G4RotationMatrix();
  theName = "GmGenerDistPosition2DDoubleGaussian"; 
}
//---------------------------------------------------------------------
G4ThreeVector GmGenerDistPosition2DDoubleGaussian::GeneratePosition( GmParticleSource* )
{
  G4double posX;
  G4double posY;
  G4double posZ = 0.;
  // X
  if( CLHEP::RandFlat::shoot() <= theProbabilityX12 ) {
    posX = sqrt(theSigmaX1M*log(1-CLHEP::RandFlat::shoot()) );
  } else {
    posX = sqrt(theSigmaX2M*log(1-CLHEP::RandFlat::shoot()) );
  }
  
  // Y
  if( CLHEP::RandFlat::shoot() <= theProbabilityY12 ) {
    posY = sqrt(theSigmaY1M*log(1-CLHEP::RandFlat::shoot()) );
  } else {
    posY = sqrt(theSigmaY2M*log(1-CLHEP::RandFlat::shoot()) );
  }
  
  G4ThreeVector pos( posX, posY, posZ );
  
  //--- Place it and rotate it
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistPosition2DDoubleGaussian::Generate pos before rotation " << pos << " rotation " << theRotation << G4endl;
#endif
  pos = theRotation * pos;
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistPosition2DDoubleGaussian::Generate pos before traslation " << pos <<" + " << theCentre << G4endl;
#endif
  pos += theCentre;

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistPosition2DDoubleGaussian::Generate pos " << pos << G4endl;
#endif
  return pos;

}


//---------------------------------------------------------------------
void GmGenerDistPosition2DDoubleGaussian::SetParams( const std::vector<G4String>& params )
{

  if( params.size() != 6 && params.size() != 9 && params.size() != 12 ) {
    G4Exception(" GmGenerDistPosition2DDoubleGaussian::SetParams",
		"Wrong argument",
		FatalErrorInArgument,
		"To set point you have to add 1, 4 or 7 parameters: SIGMA (POS_X POS_Y POS_Z) (DIR_X DIR_Y DIR_Z");
  }
  G4double sigma = GmGenUtils::GetValue( params[0] )*sqrt(2.);
  theSigmaX1M = -2.*sigma*sigma;
  sigma = GmGenUtils::GetValue( params[1] )*sqrt(2.);
  theSigmaX2M = -2.*sigma*sigma;
  theProbabilityX12 =  GmGenUtils::GetValue( params[2] );
  sigma = GmGenUtils::GetValue( params[3] )*sqrt(2.);
  theSigmaY1M = -2.*sigma*sigma;
  sigma = GmGenUtils::GetValue( params[4] )*sqrt(2.);
  theSigmaY2M = -2.*sigma*sigma;
  theProbabilityY12 =  GmGenUtils::GetValue( params[5] );
  
  if( params.size() >= 7) {
    theCentre = G4ThreeVector(GmGenUtils::GetValue( params[6] ), GmGenUtils::GetValue( params[7] ), GmGenUtils::GetValue( params[8] ) );
  }
  if( params.size() >= 10) {
    //normalize direction cosines
    G4ThreeVector dir(GmGenUtils::GetValue( params[9] ), GmGenUtils::GetValue( params[10] ), GmGenUtils::GetValue( params[11] ) );
    SetRotation( dir );
  }

}
