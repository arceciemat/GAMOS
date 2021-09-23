#ifndef GmGenerDistPositionInMaterialPhantomVoxels_HH
#define GmGenerDistPositionInMaterialPhantomVoxels_HH

#include "GamosCore/GamosGenerator/include/GmVGenerDistPosition.hh"
#include "G4ThreeVector.hh"
#include <vector>
#include <set>
class GmReadPhantomStMgr;

class GmGenerDistPositionInMaterialPhantomVoxels : public GmVGenerDistPosition
{
public:
  GmGenerDistPositionInMaterialPhantomVoxels();
  virtual ~GmGenerDistPositionInMaterialPhantomVoxels(){};

  virtual G4ThreeVector GeneratePosition( const GmParticleSource* source );
  void SetParams( const std::vector<G4String>& params );
  
private:
  void BuildVoxelPositions();

private:
  G4double theVoxelDimX;
  G4double theVoxelDimY;
  G4double theVoxelDimZ;
  std::vector<G4ThreeVector> theVoxelPositions;
  std::set<G4int> theMateIDs;
  std::vector<G4String> theMateNames;
  G4bool bInitialised;
};

#endif
