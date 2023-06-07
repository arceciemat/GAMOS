#ifndef GmGenerDistDirectionCone2DGaussian_HH
#define GmGenerDistDirectionCone2DGaussian_HH

#include "GamosCore/GamosGenerator/include/GmVGenerDistDirection.hh"
#include "G4ThreeVector.hh"
class GmParticleSource;

class GmGenerDistDirectionCone2DGaussian : public GmVGenerDistDirection
{
public:
  GmGenerDistDirectionCone2DGaussian();
  virtual ~GmGenerDistDirectionCone2DGaussian(){};

  virtual G4ThreeVector GenerateDirection( GmParticleSource* source );

  virtual void SetParams( const std::vector<G4String>& params );
  virtual void SetDirection( G4ThreeVector dir ) {
    theAxisDir = dir;
  }
  
private:
  G4ThreeVector theAxisDir;
  G4double theSigmaOpeningAngleXM;
  G4double theSigmaOpeningAngleX2;
  G4double theSigmaOpeningAngleY2;
  G4double theSigmaOpeningAngleXY2;
  G4ThreeVector thePerpDir;


};

#endif
