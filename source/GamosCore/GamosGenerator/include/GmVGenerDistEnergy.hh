#ifndef GmVGenerDistEnergy_HH
#define GmVGenerDistEnergy_HH

#include "globals.hh"
#include "GmVGenerDist.hh"
class GmParticleSource;

class GmVGenerDistEnergy : public GmVGenerDist
{
public:
  GmVGenerDistEnergy(){};
  virtual ~GmVGenerDistEnergy(){};

  virtual G4double GenerateEnergy( const GmParticleSource* source ) = 0;

};

#endif
