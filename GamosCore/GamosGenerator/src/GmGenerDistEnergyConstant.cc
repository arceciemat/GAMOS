#include "GmGenerDistEnergyConstant.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosGenerator/include/GmParticleSource.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "G4UnitsTable.hh"

//---------------------------------------------------------------------
G4double GmGenerDistEnergyConstant::GenerateEnergy( const GmParticleSource* )
{
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistEnergyConstant::Generate  energy " << theEnergy << G4endl;
#endif
  return theEnergy;

}


//---------------------------------------------------------------------
void GmGenerDistEnergyConstant::SetParams( const std::vector<G4String>& params )
{
  if( params.size() != 1 ) {
    G4Exception("GmGenerDistEnergyConstant::SetParams",
		"Wrong argument",
		FatalErrorInArgument,
		"One parameter should be given: ENERGY");
  }

  theEnergy = GmGenUtils::GetValue( params[0] );

}
