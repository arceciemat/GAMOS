#ifndef GmVGenerDistPosition_HH
#define GmVGenerDistPosition_HH

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "GmVGenerDist.hh"
class GmParticleSource;

class GmVGenerDistPosition : public GmVGenerDist
{
public:
  GmVGenerDistPosition(){};
  virtual ~GmVGenerDistPosition(){};

  virtual G4ThreeVector GeneratePosition( const GmParticleSource* source ) = 0;

};

#endif
