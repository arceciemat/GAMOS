#ifndef GmGenerDistPositionInPhantomVoxels_HH
#define GmGenerDistPositionInPhantomVoxels_HH

#include "GamosCore/GamosGenerator/include/GmVGenerDistPosition.hh"
#include "G4ThreeVector.hh"
#include <vector>

class GmGenerDistPositionInPhantomVoxels : public GmVGenerDistPosition
{
public:
  GmGenerDistPositionInPhantomVoxels();
  virtual ~GmGenerDistPositionInPhantomVoxels(){};

  virtual G4ThreeVector GeneratePosition( const GmParticleSource* source );

private:
  G4double theVoxelDimX;
  G4double theVoxelDimY;
  G4double theVoxelDimZ;
  std::vector<G4ThreeVector> theVoxelPositions;
};

#endif
