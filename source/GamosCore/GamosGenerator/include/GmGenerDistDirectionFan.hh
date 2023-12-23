#ifndef GmGenerDistDirectionFan_HH
#define GmGenerDistDirectionFan_HH

#include "GamosCore/GamosGenerator/include/GmVGenerDistDirection.hh"
#include "G4ThreeVector.hh"
class GmParticleSource;

class GmGenerDistDirectionFan : public GmVGenerDistDirection
{
public:
  GmGenerDistDirectionFan();
  virtual ~GmGenerDistDirectionFan(){};

  virtual G4ThreeVector GenerateDirection( GmParticleSource* source );

  virtual void SetParams( const std::vector<G4String>& params );
  virtual void SetDirection( G4ThreeVector dir );

private:
  G4ThreeVector theAxisDir;
  G4double theStartAngle;
  G4double theOpeningAngle;
  G4ThreeVector thePerpDir;


};

#endif
