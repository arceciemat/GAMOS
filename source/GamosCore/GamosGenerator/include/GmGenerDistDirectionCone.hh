#ifndef GmGenerDistDirectionCone_HH
#define GmGenerDistDirectionCone_HH

#include "GamosCore/GamosGenerator/include/GmVGenerDistDirection.hh"
#include "G4ThreeVector.hh"
class GmParticleSource;

class GmGenerDistDirectionCone : public GmVGenerDistDirection
{
public:
  GmGenerDistDirectionCone();
  virtual ~GmGenerDistDirectionCone(){};

  virtual G4ThreeVector GenerateDirection( const GmParticleSource* source );

  virtual void SetParams( const std::vector<G4String>& params );

private:
  G4ThreeVector theInitialDir;
  G4double theOpeningAngle;
  G4ThreeVector thePerpDir;


};

#endif
