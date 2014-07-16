#ifndef GmGenerDistDirectionRandom_HH
#define GmGenerDistDirectionRandom_HH

#include "GamosCore/GamosGenerator/include/GmVGenerDistDirection.hh"
class GmParticleSource;

class GmGenerDistDirectionRandom : public GmVGenerDistDirection
{
public:
  GmGenerDistDirectionRandom(){};
  virtual ~GmGenerDistDirectionRandom(){};

  virtual G4ThreeVector GenerateDirection( const GmParticleSource* source );

};

#endif
