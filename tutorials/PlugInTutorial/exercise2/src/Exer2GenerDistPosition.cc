#include "Exer2GenerDistPosition.hh"
#include "GamosCore/GamosGenerator/include/GmGenerVerbosity.hh"
#include "GamosCore/GamosGenerator/include/GmParticleSource.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "CLHEP/Random/RandFlat.h"

//---------------------------------------------------------------------
G4ThreeVector Exer2GenerDistPosition::GeneratePosition( const GmParticleSource* )
{
  G4double phi = CLHEP::RandFlat::shoot() * M_PI;
  
  G4ThreeVector thePoint( -0., theRadius*cos(phi), theRadius*sin(phi));
  return thePoint;

}


//---------------------------------------------------------------------
void Exer2GenerDistPosition::SetParams( const std::vector<G4String>& params )
{
  if( params.size() != 1 ) {
    G4Exception("Exer2GenerDistPosition::SetParams",
		"Wrong argument",
		FatalErrorInArgument,
		"to set point you have to add 1 parameter: RADIUS");
  }
  theRadius = GmGenUtils::GetVal( params[0] );

  G4cout << GenerVerb(infoVerb) << "Exer2GenerDistPosition::SetParams radius " << theRadius << G4endl;
  

}
