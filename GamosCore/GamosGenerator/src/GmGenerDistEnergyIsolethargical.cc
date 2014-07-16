#include "GmGenerDistEnergyIsolethargical.hh"
#include "GmGenerVerbosity.hh"

#include "CLHEP/Random/RandFlat.h"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"

//-----------------------------------------------------------------------
GmGenerDistEnergyIsolethargical::GmGenerDistEnergyIsolethargical()
{

}


//-----------------------------------------------------------------------
G4double GmGenerDistEnergyIsolethargical::GenerateEnergy( const GmParticleSource* )
{
  G4double energy = theEnergyMin * pow(theEnergyRatio, CLHEP::RandFlat::shoot());
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(debugVerb) ) G4cout << "GmGenerDistEnergyIsolethargical energy = " << energy << G4endl;
#endif
  
  return energy;

}

//---------------------------------------------------------------------
void GmGenerDistEnergyIsolethargical::SetParams( const std::vector<G4String>& params )
{
  if( params.size() != 2 ) {
    G4Exception("GmGenerDistEnergyIsolethargical::SetParams",
		 "Wrong number of parameters",
		 FatalErrorInArgument,
		 G4String("There should be two parameters: ENERGY_MIN ENERGY_MAX, while there are "+ GmGenUtils::itoa(params.size())).c_str());
  }
  theEnergyMin = GmGenUtils::GetValue(params[0]);
  theEnergyMax = GmGenUtils::GetValue(params[1]);  

  theEnergyRatio = theEnergyMax / theEnergyMin;
}
