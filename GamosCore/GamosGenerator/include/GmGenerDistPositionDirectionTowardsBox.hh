#ifndef GmGenerDistPositionDirectionTowardsBox_HH
#define GmGenerDistPositionDirectionTowardsBox_HH

#include "GamosCore/GamosGenerator/include/GmVGenerDistPosition.hh"
#include "GamosCore/GamosGenerator/include/GmVGenerDistDirection.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include <vector>
class GmTouchable;

struct TouchInfo{
  G4String name;
  G4double minX;
  G4double maxX;
  G4double minY;
  G4double maxY;
  G4ThreeVector dir;
  G4double stereoAngle;
  G4RotationMatrix rotmPlane;
};

class GmParticleSource;

class GmGenerDistPositionDirectionTowardsBox : public GmVGenerDistPosition, public GmVGenerDistDirection
{
public:
  GmGenerDistPositionDirectionTowardsBox();
  virtual ~GmGenerDistPositionDirectionTowardsBox(){};

  virtual G4ThreeVector GeneratePosition( const GmParticleSource* source );
  virtual G4ThreeVector GenerateDirection( const GmParticleSource* source );

  virtual void SetParams( const std::vector<G4String>& params );

private:
  void SetDirCosineLimits( const GmTouchable* touch );

private:
  G4ThreeVector thePoint;
  std::vector< TouchInfo* > theTouchInfos;
  G4double theTotalStereoAngle;
};

#endif
