#ifndef GmGenerDistPositionPoint_HH
#define GmGenerDistPositionPoint_HH

#include "GamosCore/GamosGenerator/include/GmVGenerDistPosition.hh"
#include "G4ThreeVector.hh"
class GmParticleSource;

class GmGenerDistPositionPoint : public GmVGenerDistPosition
{
public:
  GmGenerDistPositionPoint(){};
  virtual ~GmGenerDistPositionPoint(){};

  virtual G4ThreeVector GeneratePosition( const GmParticleSource* source );

  virtual void SetParams( const std::vector<G4String>& params );

private:
  G4ThreeVector thePoint;

};

#endif
