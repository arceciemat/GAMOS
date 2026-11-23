#include "GmGenerDistTimeConstantChangeRandom.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosGenerator/include/GmParticleSource.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "CLHEP/Random/RandFlat.h"
#include "G4UnitsTable.hh"

//---------------------------------------------------------------------
GmGenerDistTimeConstantChangeRandom::GmGenerDistTimeConstantChangeRandom()
{
  thePreviousTime = 0.;
  theInterval = 0.;
}

//---------------------------------------------------------------------
G4double GmGenerDistTimeConstantChangeRandom::GenerateTime( const GmParticleSource* )
{
  G4double time = theInterval*CLHEP::RandFlat::shoot();
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistTimeConstantChangeRandom::GenerateTime " << thePreviousTime+time << " theInterval " << theInterval << G4endl;
#endif
  return thePreviousTime+time;
}


//---------------------------------------------------------------------
void GmGenerDistTimeConstantChangeRandom::SetParams( const std::vector<G4String>& params )
{
  if( params.size() != 1 &&  params.size() != 2 ) {
    G4Exception("GmGenerDistTimeConstantChangeRandom::SetParams",
		"Wrong argument",
		FatalErrorInArgument,
		"One or tow parameters should be defined: TIME_INTERVAL (INITAL_TIME)");
  }

  theInterval = GmGenUtils::GetValue( params[0] );

  if( params.size() == 2 ) {
    thePreviousTime = GmGenUtils::GetValue( params[1] );
  }
}
