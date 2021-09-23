#ifndef GmGenerDistPositionInStPhantomVoxels_HH
#define GmGenerDistPositionInStPhantomVoxels_HH

#include "GamosCore/GamosGenerator/include/GmVGenerDistPosition.hh"
#include "G4ThreeVector.hh"
#include <vector>
#include <set>
class GmReadPhantomStMgr;

class GmGenerDistPositionInStPhantomVoxels : public GmVGenerDistPosition
{
public:
  GmGenerDistPositionInStPhantomVoxels();
  virtual ~GmGenerDistPositionInStPhantomVoxels(){};

  virtual G4ThreeVector GeneratePosition( const GmParticleSource* source );
  void SetParams( const std::vector<G4String>& params );
  
private:
  void BuildVoxelPositions();

private:
  G4double theVoxelDimX;
  G4double theVoxelDimY;
  G4double theVoxelDimZ;
  std::vector<G4ThreeVector> theVoxelPositions;
  GmReadPhantomStMgr* theReadPhantomStMgr;
  std::set<G4int> theStIDs;
  
};

#endif
