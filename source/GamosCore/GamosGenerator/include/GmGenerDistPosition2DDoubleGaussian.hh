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
  /*  G4double GetSigma() const {
    return theSigmaM;
  }
  void SetSigma( G4double sig ) {
    //    G4cout << " SetSigma " << sig << G4endl; //GDEB
    theSigmaM = -2*sig*sig;
    }*/

private:
  G4double theSigmaX1M;
  G4double theSigmaX2M;
  G4double theProbabilityX12;
  G4double theSigmaY1M;
  G4double theSigmaY2M;
  G4double theProbabilityY12;
};

#endif
