#include "GmGenerDistEnergyRandomFlat.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosGenerator/include/GmParticleSource.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "CLHEP/Random/RandFlat.h"
#include "G4UnitsTable.hh"


G4double GmGenerDistEnergyRandomFlat::GenerateEnergy( const GmParticleSource* )
{
 G4double ener = CLHEP::RandFlat::shoot( theEnergyMin, theEnergyMax );
#ifndef GAMOS_NO_VERBOSE
 if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistEnergyRandomFlat::Generate  energy " << ener << " " << theEnergyMin<< " " << theEnergyMax <<  G4endl;
#endif
 return ener;
}

void GmGenerDistEnergyRandomFlat::SetParams( const std::vector<G4String>& params )
{

  if( params.size() != 2 ) {
    G4Exception(" GmGenerDistEnergyRandomFlat::SetParams",
		"Wrong argument",
		FatalErrorInArgument,
		"There should be two parameters: energyMin energyMax");
  }

  theEnergyMin = GmGenUtils::GetValue( params[0] );
  theEnergyMax = GmGenUtils::GetValue( params[1] );
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistEnergyRandomFlat::SetParams " << theEnergyMin << " " <<  params[0] << " " <<  G4UnitDefinition::GetValueOf(params[1]) << G4endl;
#endif
}
