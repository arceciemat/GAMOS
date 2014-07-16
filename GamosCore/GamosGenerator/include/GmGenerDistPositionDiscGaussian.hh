#ifndef GmGenerDistPositionDiscGaussian_HH
#define GmGenerDistPositionDiscGaussian_HH

#include "GamosCore/GamosGenerator/include/GmVGenerDistPosition.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
class GmParticleSource;

class GmGenerDistPositionDiscGaussian : public GmVGenerDistPosition
{
public:
  GmGenerDistPositionDiscGaussian(){};
  virtual ~GmGenerDistPositionDiscGaussian(){};

  virtual G4ThreeVector GeneratePosition( const GmParticleSource* source );

  virtual void SetParams( const std::vector<G4String>& params );

private:
  G4double theSigmaM;
  G4ThreeVector theCentre;
  G4RotationMatrix theRotation;
};

#endif
