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

  virtual G4ThreeVector GenerateDirection( GmParticleSource* source );

  virtual void SetParams( const std::vector<G4String>& params );
  virtual void SetDirection( G4ThreeVector dir ) {
    theAxisDir = dir;
  }
  

private:
  G4ThreeVector theAxisDir;
  G4double theSigmaOpeningAngleC;
  G4ThreeVector thePerpDir;


};

#endif
