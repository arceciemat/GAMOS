#ifndef GmGenerDistPositionInG4VolumesGeneral_HH
#define GmGenerDistPositionInG4VolumesGeneral_HH

#include "GamosCore/GamosGenerator/include/GmVGenerDistPosition.hh"
#include "G4ThreeVector.hh"
#include "G4TouchableHistory.hh"
class GmParticleSource;
class GmGeometryUtils;

class GmGenerDistPositionInG4VolumesGeneral : public GmVGenerDistPosition
{
public:
  GmGenerDistPositionInG4VolumesGeneral();
  virtual ~GmGenerDistPositionInG4VolumesGeneral(){delete theTouchable;};

  virtual G4ThreeVector GeneratePosition( GmParticleSource* source );

  virtual void SetParams( const std::vector<G4String>& params );

private:
  void SetWorldDimensions();

private:
  G4double theWorldHalfX, theWorldHalfY, theWorldHalfZ;
  std::vector<G4String> theVolumes;
  G4TouchableHistory* theTouchable;

  GmGeometryUtils* theGeomUtils;
};

#endif
