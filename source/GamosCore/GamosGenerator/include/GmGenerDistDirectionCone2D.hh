#ifndef GmGenerDistDirectionCone2D_HH
#define GmGenerDistDirectionCone2D_HH

#include "GamosCore/GamosGenerator/include/GmVGenerDistDirection.hh"
#include "G4ThreeVector.hh"
class GmParticleSource;

class GmGenerDistDirectionCone2D : public GmVGenerDistDirection
{
public:
  GmGenerDistDirectionCone2D();
  virtual ~GmGenerDistDirectionCone2D(){};

  virtual G4ThreeVector GenerateDirection( const GmParticleSource* source );

  virtual void SetParams( const std::vector<G4String>& params );
  void SetDirection( G4ThreeVector dir ) {
    theInitialDir = dir;
  }
  
private:
  G4ThreeVector theInitialDir;
  G4double theOpeningAngleX2;
  G4double theOpeningAngleY2;
  G4ThreeVector thePerpDir;

};

#endif
