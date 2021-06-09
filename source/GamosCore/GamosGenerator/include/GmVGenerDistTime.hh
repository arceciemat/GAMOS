#ifndef GmVGenerDistTime_HH
#define GmVGenerDistTime_HH

#include "globals.hh"
#include "GmVGenerDist.hh"
class GmParticleSource;

class GmVGenerDistTime : public GmVGenerDist
{
public:
  GmVGenerDistTime(){};
  virtual ~GmVGenerDistTime(){};

  virtual G4double GenerateTime( const GmParticleSource* source ) = 0;

};

#endif
