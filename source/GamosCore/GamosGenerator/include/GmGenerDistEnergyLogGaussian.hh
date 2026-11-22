#ifndef GmGenerDistEnergyLogGaussian_HH
#define GmGenerDistEnergyLogGaussian_HH

// Author: M.A. Cortes  /06/2008

#include "GamosCore/GamosGenerator/include/GmVGenerDistEnergy.hh"
class GmParticleSource;

class GmGenerDistEnergyLogGaussian : public GmVGenerDistEnergy
{
public:
  GmGenerDistEnergyLogGaussian(){};
  virtual ~GmGenerDistEnergyLogGaussian(){};

  virtual G4double GenerateEnergy( const GmParticleSource* source );

  virtual void SetParams( const std::vector<G4String>& params );

private:
  G4double theMeanEnergy, theSigma, theInverse;
};

#endif
