#ifndef GmGenerDistDirectionConeGaussian_HH
#define GmGenerDistDirectionConeGaussian_HH

#include "GamosCore/GamosGenerator/include/GmVGenerDistDirection.hh"
#include "G4ThreeVector.hh"
class GmParticleSource;

class GmGenerDistDirectionConeGaussian : public GmVGenerDistDirection
{
public:
  GmGenerDistDirectionConeGaussian();
  virtual ~GmGenerDistDirectionConeGaussian(){};

  virtual G4ThreeVector GenerateDirection( const GmParticleSource* source );

  virtual void SetParams( const std::vector<G4String>& params );
  void SetDirection( G4ThreeVector dir ) {
    theInitialDir = dir;
  }
  

private:
  G4ThreeVector theInitialDir;
  G4double theSigmaOpeningAngleC;
  G4ThreeVector thePerpDir;


};

#endif
