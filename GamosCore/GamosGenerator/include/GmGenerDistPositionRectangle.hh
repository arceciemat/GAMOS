#ifndef GmGenerDistPositionRectangle_HH
#define GmGenerDistPositionRectangle_HH

#include "GmVGenerDistPosition2D.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
class GmParticleSource;

class GmGenerDistPositionRectangle : public GmVGenerDistPosition2D
{
public:
  GmGenerDistPositionRectangle(){};
  virtual ~GmGenerDistPositionRectangle(){};

  virtual G4ThreeVector GeneratePosition( const GmParticleSource* source );

  virtual void SetParams( const std::vector<G4String>& params );
  
private:
  G4double theWidthX;
  G4double theWidthY;
};

#endif
