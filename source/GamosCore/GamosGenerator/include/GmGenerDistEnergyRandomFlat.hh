#ifndef GmGenerDistEnergyRandomFlat_HH
#define GmGenerDistEnergyRandomFlat_HH

#include "GamosCore/GamosGenerator/include/GmVGenerDistEnergy.hh"
class GmParticleSource;

class GmGenerDistEnergyRandomFlat : public GmVGenerDistEnergy
{
public:
  GmGenerDistEnergyRandomFlat(){};
  virtual ~GmGenerDistEnergyRandomFlat(){};

  virtual G4double GenerateEnergy( const GmParticleSource* source );

  virtual void SetParams( const std::vector<G4String>& params );

private:
  G4double theEnergyMin, theEnergyMax;
};

#endif
