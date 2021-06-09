#ifndef GmGenerDistDirectionConst_HH
#define GmGenerDistDirectionConst_HH

#include "GamosCore/GamosGenerator/include/GmVGenerDistDirection.hh"
#include "G4ThreeVector.hh"
class GmParticleSource;

class GmGenerDistDirectionConst : public GmVGenerDistDirection
{
public:
  GmGenerDistDirectionConst();
  virtual ~GmGenerDistDirectionConst(){};

  virtual G4ThreeVector GenerateDirection( const GmParticleSource* source );

  virtual void SetParams( const std::vector<G4String>& params );

private:
  G4ThreeVector theDir;

};

#endif
