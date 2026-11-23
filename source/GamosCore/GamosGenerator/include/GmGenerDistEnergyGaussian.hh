#ifndef GmGenerDistEnergyGaussian_HH
#define GmGenerDistEnergyGaussian_HH

// Author: M.A. Cortes  /06/2008

#include "GamosCore/GamosGenerator/include/GmVGenerDistEnergy.hh"
class GmParticleSource;

class GmGenerDistEnergyGaussian : public GmVGenerDistEnergy
{
public:
  GmGenerDistEnergyGaussian(){};
  virtual ~GmGenerDistEnergyGaussian(){};

  virtual G4double GenerateEnergy( const GmParticleSource* source );

  virtual void SetParams( const std::vector<G4String>& params );

private:
  G4double theMeanEnergy, theSigma;
};

#endif
