#ifndef GmGenerDistPositionLineSteps_HH
#define GmGenerDistPositionLineSteps_HH

#include "GamosCore/GamosGenerator/include/GmVGenerDistPosition.hh"
#include "G4ThreeVector.hh"
class GmParticleSource;

class GmGenerDistPositionLineSteps : public GmVGenerDistPosition
{
public:
  GmGenerDistPositionLineSteps();
  virtual ~GmGenerDistPositionLineSteps(){};

  virtual G4ThreeVector GeneratePosition( const GmParticleSource* source );

  virtual void SetParams( const std::vector<G4String>& params );

private:
  G4ThreeVector thePoint;
  G4ThreeVector theDirection;
  G4double theStep;
  G4int nSteps;
};

#endif
