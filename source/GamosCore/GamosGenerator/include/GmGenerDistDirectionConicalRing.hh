#ifndef GmGenerDistDirectionConicalRing_HH
#define GmGenerDistDirectionConicalRing_HH

#include "GamosCore/GamosGenerator/include/GmVGenerDistDirection.hh"
#include "G4ThreeVector.hh"
class GmParticleSource;

class GmGenerDistDirectionConicalRing : public GmVGenerDistDirection
{
public:
  GmGenerDistDirectionConicalRing();
  virtual ~GmGenerDistDirectionConicalRing(){};

  virtual G4ThreeVector GenerateDirection( GmParticleSource* source );

  virtual void SetParams( const std::vector<G4String>& params );
  virtual void SetDirection( G4ThreeVector dir );

private:
  G4ThreeVector theAxisDir;
  G4double theInitialAngle;
  G4double theOpeningAngle;
  G4double theCoveredAngle;
  G4ThreeVector thePerpDir;

};

#endif
