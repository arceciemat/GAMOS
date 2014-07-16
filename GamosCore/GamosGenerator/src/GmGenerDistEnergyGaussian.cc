#include "GmGenerDistEnergyGaussian.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosGenerator/include/GmParticleSource.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "CLHEP/Random/RandGauss.h"
#include "CLHEP/Units/SystemOfUnits.h"


G4double GmGenerDistEnergyGaussian::GenerateEnergy( const GmParticleSource* )
{
 G4double ener = theSigma * CLHEP::RandGauss::shoot();
 ener += theMeanEnergy;

#ifndef GAMOS_NO_VERBOSE
 if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistEnergyGaussian::Generate energy " << ener/CLHEP::MeV << " MeV with mean kinetic energy = " << theMeanEnergy/CLHEP::MeV << " MeV and sigma = " << theSigma/CLHEP::MeV << " MeV "<< G4endl;
#endif
 return ener;
}

void GmGenerDistEnergyGaussian::SetParams( const std::vector<G4String>& params )
{

  if( params.size() != 2 ) {
    G4Exception(" GmGenerDistEnergyGaussian::SetParams",
		"Wrong argument",
		FatalErrorInArgument,
		"There should be two parameters: MEAN_ENERGY SIGMA");
  }

  theMeanEnergy = GmGenUtils::GetValue( params[0] );
  theSigma = GmGenUtils::GetValue( params[1] );
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistEnergyGaussian::SetParams theMeanEnergy = " << theMeanEnergy/CLHEP::MeV << " MeV and theSigma = " << theSigma/CLHEP::MeV << " MeV" << G4endl;
#endif
}
