#ifndef Exer2GenerDistPosition_HH
#define Exer2GenerDistPosition_HH

#include "GamosCore/GamosGenerator/include/GmVGenerDistPosition.hh"
class GmParticleSource;

class Exer2GenerDistPosition : public GmVGenerDistPosition
{
public:
  Exer2GenerDistPosition(){};
  virtual ~Exer2GenerDistPosition(){};

  virtual G4ThreeVector GeneratePosition( const GmParticleSource* source );

  virtual void SetParams( const std::vector<G4String>& params );

private:
  G4double theRadius;
};

#endif

