#ifndef GmVGenerDistDirection_HH
#define GmVGenerDistDirection_HH

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "GmVGenerDist.hh"
class GmParticleSource;

class GmVGenerDistDirection : public GmVGenerDist
{
public:
  GmVGenerDistDirection(){};
  virtual ~GmVGenerDistDirection(){};

  virtual G4ThreeVector GenerateDirection( const GmParticleSource* source ) = 0;


};

#endif
