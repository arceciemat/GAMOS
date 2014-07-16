#ifndef GmVGenerDistPosition2D_HH
#define GmVGenerDistPosition2D_HH

#include "GamosCore/GamosGenerator/include/GmVGenerDistPosition.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
class GmParticleSource;

class GmVGenerDistPosition2D : public GmVGenerDistPosition
{
public:
  GmVGenerDistPosition2D(){};
  virtual ~GmVGenerDistPosition2D(){};

  virtual G4ThreeVector GeneratePosition( const GmParticleSource* source ) = 0;

  virtual G4ThreeVector RotateAndTranslate( G4ThreeVector& posini );

protected:
  G4ThreeVector theCentre;
  G4RotationMatrix theRotation;
};

#endif
