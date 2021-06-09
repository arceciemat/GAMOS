#include "GmGenerDistEnergyConstantIsotopeDecay.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosGenerator/include/GmParticleSource.hh"
#include "GamosCore/GamosGenerator/include/GmIsotopeSource.hh"
#include "G4UnitsTable.hh"

//---------------------------------------------------------------------
G4double GmGenerDistEnergyConstantIsotopeDecay::GenerateEnergy( const GmParticleSource* source )
{ 
  if( source->GetType() != "IsotopeSource" ) {
    G4Exception(" GmGenerDistEnergyConstantIsotopeDecay::GenerateEnergy",
		"Wrong argument",
		FatalErrorInArgument,
		"Source is not isotope");
  }

  GmIsotopeSource* isoSource = static_cast<GmIsotopeSource*>(const_cast<GmParticleSource*>(source));
  G4double energy = isoSource->GetCurrentDecay()->GetEnergy();
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistEnergyConstantIsotopeDecay::Generate  energy " << energy << G4endl;
#endif
  return energy;

}
