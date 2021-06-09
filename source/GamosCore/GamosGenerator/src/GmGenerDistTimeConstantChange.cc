#include "GmGenerDistTimeConstantChange.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosGenerator/include/GmParticleSource.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "CLHEP/Random/RandFlat.h"
#include "G4UnitsTable.hh"

//---------------------------------------------------------------------
GmGenerDistTimeConstantChange::GmGenerDistTimeConstantChange()
{
  thePreviousTime = 0.;
  theInterval = 0.;
}

//---------------------------------------------------------------------
G4double GmGenerDistTimeConstantChange::GenerateTime( const GmParticleSource* )
{
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << " GmGenerDistTimeConstantChange::GenerateTime " << thePreviousTime+theInterval << G4endl;
#endif
 return thePreviousTime+theInterval;
}


//---------------------------------------------------------------------
void GmGenerDistTimeConstantChange::SetParams( const std::vector<G4String>& params )
{
  if( params.size() != 1 &&  params.size() != 2 ) {
    G4Exception("GmGenerDistTimeConstantChange::SetParams",
		"Wrong argument",
		FatalErrorInArgument,
		"One or tow parameters should be defined: TIME_INTERVAL (INITAL_TIME)");
  }

  theInterval = GmGenUtils::GetValue( params[0] );

  if( params.size() == 2 ) {
    thePreviousTime = GmGenUtils::GetValue( params[1] );
  }
}
