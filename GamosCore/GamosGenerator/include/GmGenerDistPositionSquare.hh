#ifndef GmGenerDistPositionSquare_HH
#define GmGenerDistPositionSquare_HH

#include "GmVGenerDistPosition2D.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
class GmParticleSource;

class GmGenerDistPositionSquare : public GmVGenerDistPosition2D
{
public:
  GmGenerDistPositionSquare(){};
  virtual ~GmGenerDistPositionSquare(){};

  virtual G4ThreeVector GeneratePosition( const GmParticleSource* source );

  virtual void SetParams( const std::vector<G4String>& params );
  
private:
  G4double theHalfWidth;
};

#endif
