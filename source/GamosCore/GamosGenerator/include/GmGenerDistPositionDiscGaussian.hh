#ifndef GmGenerDistPositionDiscGaussian_HH
#define GmGenerDistPositionDiscGaussian_HH

#include "GamosCore/GamosGenerator/include/GmVGenerDistPosition2D.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
class GmParticleSource;

class GmGenerDistPositionDiscGaussian : public GmVGenerDistPosition2D
{
public:
  GmGenerDistPositionDiscGaussian();
  virtual ~GmGenerDistPositionDiscGaussian(){};

  virtual G4ThreeVector GeneratePosition( GmParticleSource* source );

  virtual void SetParams( const std::vector<G4String>& params );
  G4double GetSigma() const {
    return theSigmaM;
  }
  void SetSigma( G4double sig ) {
    //    G4cout << " SetSigma " << sig << G4endl; //GDEB
    theSigmaM = -2*sig*sig;
  }

private:
  G4double theSigmaM;
};

#endif
