#ifndef GmGenerDistPosition2DDoubleGaussian_HH
#define GmGenerDistPosition2DDoubleGaussian_HH

#include "GamosCore/GamosGenerator/include/GmVGenerDistPosition2D.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
class GmParticleSource;

class GmGenerDistPosition2DDoubleGaussian : public GmVGenerDistPosition2D
{
public:
  GmGenerDistPosition2DDoubleGaussian();
  virtual ~GmGenerDistPosition2DDoubleGaussian(){};

  virtual G4ThreeVector GeneratePosition( GmParticleSource* source );

  virtual void SetParams( const std::vector<G4String>& params );

private:
  G4double theSigmaX1;
  G4double theSigmaX2;
  G4double theProbabilityX12;
  G4double theSigmaY1;
  G4double theSigmaY2;
  G4double theProbabilityY12;
};

#endif
