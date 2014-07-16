#include "GmGenerDistTimeConstant.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosGenerator/include/GmParticleSource.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "CLHEP/Random/RandFlat.h"
#include "G4UnitsTable.hh"

//---------------------------------------------------------------------
GmGenerDistTimeConstant::GmGenerDistTimeConstant()
{
  theTime = 0.;
}

//---------------------------------------------------------------------
G4double GmGenerDistTimeConstant::GenerateTime( const GmParticleSource* )
{
  return theTime;
}


//---------------------------------------------------------------------
void GmGenerDistTimeConstant::SetParams( const std::vector<G4String>& params )
{
  if( params.size() == 0 ) {
    return;
  } else if( params.size() != 1 ) {
    G4Exception("GmGenerDistTimeConstant::SetParams",
		"Wrong argument",
		FatalErrorInArgument,
		"Only one parameter should be defined: TIME");
  }

  theTime = GmGenUtils::GetValue( params[0] );

}
