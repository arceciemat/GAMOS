#include "GmGenerDistDirectionGaussian.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosGenerator/include/GmParticleSource.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "CLHEP/Random/RandGauss.h"

G4ThreeVector GmGenerDistDirectionGaussian::GenerateDirection( const GmParticleSource* )
{
  G4double dirX = CLHEP::RandGauss::shoot( 0., theSigmaX );
  G4double dirY = CLHEP::RandGauss::shoot( 0., theSigmaY );

  G4ThreeVector dir = G4ThreeVector( dirX, dirY, 1 );

  dir.rotateZ( theTheta );
  dir.rotateX( thePhi );

#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistDirectionGaussian::Generate  dir "  << dir << G4endl;
#endif
  return dir;

}
//---------------------------------------------------------------------
void GmGenerDistDirectionGaussian::SetParams( const std::vector<G4String>& params )
{
  if( params.size() != 5 ) {
    G4Exception("GmGenerDistDirectionConst::SetParam",
		"Wrong argument",
		FatalErrorInArgument,
		"To set direction you have to add 3 parameters: DIR_X DIR_Y DIR_Z SIGMA_Y SIGMA_Z");
  }
  
  G4ThreeVector dir = G4ThreeVector(GmGenUtils::GetValue( params[0] ), GmGenUtils::GetValue( params[1] ), GmGenUtils::GetValue( params[2] ) );
  thePhi = dir.phi();
  theTheta = dir.theta();
  theSigmaX = GmGenUtils::GetValue( params[3] );
  theSigmaY = GmGenUtils::GetValue( params[4] );
}
