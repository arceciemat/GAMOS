#ifndef GmGenerDistPositionDirectionInUserSurfaces_HH
#define GmGenerDistPositionDirectionInUserSurfaces_HH

#include "GamosCore/GamosGenerator/include/GmGenerDistPositionInUserSurfaces.hh"
#include "GamosCore/GamosGenerator/include/GmVGenerDistDirection.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"

class GmGenerDistPositionDirectionInUserSurfaces : public GmGenerDistPositionInUserSurfaces, public GmVGenerDistDirection
{
public:
  GmGenerDistPositionDirectionInUserSurfaces();
  virtual ~GmGenerDistPositionDirectionInUserSurfaces(){};

  void SetParams( const std::vector<G4String>& params );
  virtual G4ThreeVector GenerateDirection( const GmParticleSource* source );

private:

  G4ThreeVector theTranslation;
  G4RotationMatrix* theRotation;
  G4bool bOutward;
};

#endif
