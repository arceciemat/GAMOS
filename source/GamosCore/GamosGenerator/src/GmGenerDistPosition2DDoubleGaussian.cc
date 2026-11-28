#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
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
    posX = CLHEP::RandGauss::shoot(0., theSigmaX1);
  } else {
    posX = CLHEP::RandGauss::shoot(0., theSigmaX2);
  }
  
  // Y
  if( CLHEP::RandFlat::shoot() <= theProbabilityY12 ) {
    posY = CLHEP::RandGauss::shoot(0., theSigmaY1);
  } else {
    posY = CLHEP::RandGauss::shoot(0., theSigmaY2);
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
		"To set the distribution you have to add 6, 9 or 12 parameters: SIGMA_X1 SIGMA_X2 PROBABILITY_X1 SIGMA_Y1 SIGMA_Y2 PROBABILITY_Y1 (POS_X POS_Y POS_Z) (DIR_X DIR_Y DIR_Z)");
  }
  theSigmaX1 = GmGenUtils::GetValue( params[0] );
  theSigmaX2 = GmGenUtils::GetValue( params[1] );
  theProbabilityX12 =  GmGenUtils::GetValue( params[2] );
  theSigmaY1 = GmGenUtils::GetValue( params[3] );
  theSigmaY2 = GmGenUtils::GetValue( params[4] );
  theProbabilityY12 =  GmGenUtils::GetValue( params[5] );
  
  if( params.size() >= 9) {
    theCentre = G4ThreeVector(GmGenUtils::GetValue( params[6] ), GmGenUtils::GetValue( params[7] ), GmGenUtils::GetValue( params[8] ) );
  }
  if( params.size() >= 12) {
    //normalize direction cosines
    G4ThreeVector dir(GmGenUtils::GetValue( params[9] ), GmGenUtils::GetValue( params[10] ), GmGenUtils::GetValue( params[11] ) );
    SetRotation( dir );
  }

}
