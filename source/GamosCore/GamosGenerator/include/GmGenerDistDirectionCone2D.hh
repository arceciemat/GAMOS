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

  virtual G4ThreeVector GenerateDirection( GmParticleSource* source );

  virtual void SetParams( const std::vector<G4String>& params );
  virtual void SetDirection( G4ThreeVector dir ) {
    theAxisDir = dir;
  }
  
private:
  G4ThreeVector theAxisDir;
  G4double theOpeningAngleX2;
  G4double theOpeningAngleY2;
  G4ThreeVector thePerpDir;

};

#endif
