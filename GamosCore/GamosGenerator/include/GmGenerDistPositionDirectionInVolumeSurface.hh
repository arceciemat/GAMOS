#ifndef GmGenerDistPositionDirectionInVolumeSurface_HH
#define GmGenerDistPositionDirectionInVolumeSurface_HH

#include "GamosCore/GamosGenerator/include/GmVGenerDistPosition.hh"
#include "GamosCore/GamosGenerator/include/GmVGenerDistDirection.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"


class GmGenerDistPositionDirectionInVolumeSurface : public GmVGenerDistPosition, public GmVGenerDistDirection
{
public:
  GmGenerDistPositionDirectionInVolumeSurface();
  virtual ~GmGenerDistPositionDirectionInVolumeSurface(){};

  virtual G4ThreeVector GeneratePosition( const GmParticleSource* source );
  virtual G4ThreeVector GenerateDirection( const GmParticleSource* source );

  virtual void SetParams( const std::vector<G4String>& params );

private:
  G4String theSolidType;
  std::vector<G4double> theParams;
  G4ThreeVector theTranslation;
  G4RotationMatrix* theRotation;

  G4ThreeVector thePosition;

};

#endif
