#ifndef GmGenerDistEnergyConstant_HH
#define GmGenerDistEnergyConstant_HH

#include "GamosCore/GamosGenerator/include/GmVGenerDistEnergy.hh"
class GmParticleSource;

class GmGenerDistEnergyConstant : public GmVGenerDistEnergy
{
public:
  GmGenerDistEnergyConstant(){};
  virtual ~GmGenerDistEnergyConstant(){};

  virtual G4double GenerateEnergy( const GmParticleSource* source );

  virtual void SetParams( const std::vector<G4String>& params );

  void SetEnergy( G4double ener ){
    theEnergy = ener; }

private:

  G4double theEnergy;
};

#endif
