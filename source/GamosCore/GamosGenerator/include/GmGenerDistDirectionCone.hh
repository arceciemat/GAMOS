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

  virtual G4ThreeVector GenerateDirection( GmParticleSource* source );

  virtual void SetParams( const std::vector<G4String>& params );
  virtual void SetDirection( G4ThreeVector dir );

private:
  G4ThreeVector theAxisDir;
  G4double theOpeningAngle;
  G4ThreeVector thePerpDir;


};

#endif
