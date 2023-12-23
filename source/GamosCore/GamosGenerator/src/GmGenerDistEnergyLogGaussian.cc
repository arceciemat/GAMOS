#include "GmGenerDistEnergyLogGaussian.hh"
#include "GmGenerVerbosity.hh"
#include "GamosCore/GamosGenerator/include/GmParticleSource.hh"
#include "GamosCore/GamosUtils/include/GmGenUtils.hh"
#include "CLHEP/Random/RandGauss.h"
#include "CLHEP/Units/SystemOfUnits.h"

#include <random>

G4double GmGenerDistEnergyLogGaussian::GenerateEnergy( const GmParticleSource* )
{
  G4double ener = 0.;
  for( int iie = 0; ; iie++ ) {
    // Semilla para el generador de números aleatorios
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    // Crear una distribución log-normal
    std::lognormal_distribution<double> lognormal_dist(theMeanEnergy, theSigma);
    // Generar muestras de la distribución log-normal
    ener = lognormal_dist(generator);
    
    if( theInverse != 0. ) {
      ener = theInverse - ener;
    }
    if( ener >= 0 ) break;
    if( iie > 1E6 ) {
      G4Exception(" GmGenerDistEnergyLogGaussian::GenerateEnergy",
		  "",
		FatalErrorInArgument,
		"More than 1E6 tries without getting energy >= 0");
    }
  }
#ifndef GAMOS_NO_VERBOSE
 if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistEnergyLogGaussian::Generate energy " << ener/CLHEP::MeV << " MeV with mean kinetic energy = " << theMeanEnergy/CLHEP::MeV << " MeV and sigma = " << theSigma/CLHEP::MeV << " MeV "<< G4endl;
#endif
 return ener;
}

void GmGenerDistEnergyLogGaussian::SetParams( const std::vector<G4String>& params )
{
  
  if( params.size() != 2 && params.size() != 3 ) { 
    G4Exception(" GmGenerDistEnergyLogGaussian::SetParams",
		"Wrong argument",
		FatalErrorInArgument,
		"There should be two parameters: MEAN_ENERGY SIGMA (INVERSE)");
  }
  
  theMeanEnergy = GmGenUtils::GetValue( params[0] );
  theSigma = GmGenUtils::GetValue( params[1] );
  theInverse = 0.;
  if( params.size() == 3 ) {
    theInverse = GmGenUtils::GetValue( params[2] );
    if( theInverse < 0. ) {
      G4Exception("GmGenerDistEnergyLogGaussian::SetParam",
		  "",
		  FatalException,
		  ("Inverse parameter cannot be < 0., it is "+GmGenUtils::ftoa(theInverse)).c_str());
    }
  }
#ifndef GAMOS_NO_VERBOSE
  if( GenerVerb(infoVerb) ) G4cout << " GmGenerDistEnergyLogGaussian::SetParams theMeanEnergy = " << theMeanEnergy/CLHEP::MeV << " MeV and theSigma = " << theSigma/CLHEP::MeV << " MeV" << G4endl;
#endif
}
