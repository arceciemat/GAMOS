#ifndef GmGenerDistPositionVoxelPhantomMaterials_HH
#define GmGenerDistPositionVoxelPhantomMaterials_HH

#include "GamosCore/GamosGenerator/include/GmVGenerDistPosition.hh"
#include "G4ThreeVector.hh"
class GmParticleSource;
class G4PhantomParameterisation;
#include "G4AffineTransform.hh"
#include <set>


class GmGenerDistPositionVoxelPhantomMaterials : public GmVGenerDistPosition
{
public:
  GmGenerDistPositionVoxelPhantomMaterials();
  virtual ~GmGenerDistPositionVoxelPhantomMaterials(){};

  virtual void SetParams( const std::vector<G4String>& params );

  virtual G4ThreeVector GeneratePosition( const GmParticleSource* source );


private:

  G4PhantomParameterisation* theParamReg;
  G4AffineTransform theTransformation;
  G4double theVoxelDimX, theVoxelDimY, theVoxelDimZ;
  unsigned int theNoVoxelX, theNoVoxelY, theNoVoxelZ;
  std::set<unsigned int> theMaterialIndexes;
};

#endif
