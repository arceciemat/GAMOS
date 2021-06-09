#ifndef GmGenerDistDirectionGaussian_HH
#define GmGenerDistDirectionGaussian_HH

#include "GamosCore/GamosGenerator/include/GmVGenerDistDirection.hh"
class GmParticleSource;

class GmGenerDistDirectionGaussian : public GmVGenerDistDirection
{
public:
  GmGenerDistDirectionGaussian(){};
  virtual ~GmGenerDistDirectionGaussian(){};

  virtual G4ThreeVector GenerateDirection( const GmParticleSource* source );

  virtual void SetParams( const std::vector<G4String>& params );

private:
  G4double theTheta;
  G4double thePhi;
  G4double theSigmaX;
  G4double theSigmaY;
};

#endif
