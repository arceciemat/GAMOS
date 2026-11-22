#ifndef GmGenerDistEnergyConstantIsotopeDecay_HH
#define GmGenerDistEnergyConstantIsotopeDecay_HH

#include "GamosCore/GamosGenerator/include/GmVGenerDistEnergy.hh"
class GmParticleSource;

class GmGenerDistEnergyConstantIsotopeDecay : public GmVGenerDistEnergy
{
public:
  GmGenerDistEnergyConstantIsotopeDecay(){};
  virtual ~GmGenerDistEnergyConstantIsotopeDecay(){};

  virtual G4double GenerateEnergy( const GmParticleSource* source );

};

#endif
