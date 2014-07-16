#ifndef GmGenerEnergyDistConstant_HH
#define GmGenerEnergyDistConstant_HH

#include "GmVGenerEnergyDist.hh"

class GmGenerEnergyDistConstant : public GamosVGenerEnergyDist
{
public:
  GmGenerEnergyDistConstant(){};
  virtual ~GmGenerEnergyDistConstant(){};

  virtual G4double GenerateEnergy( G4double activity );

};

#endif
