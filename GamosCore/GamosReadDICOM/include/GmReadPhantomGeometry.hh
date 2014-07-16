#ifndef GmReadPhantomGeometry_h
#define GmReadPhantomGeometry_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4ThreeVector.hh"

class G4Material;
class G4LogicalVolume;
class G4PhysicalVolume;
class G4Box;
class G4PhantomParameterisation;
#include <map>

struct matInfo 
{
  G4double sumdens;
  G4int nvoxels;
  G4int id;
};

class GmReadPhantomGeometry : public G4VUserDetectorConstruction
{
public:

  GmReadPhantomGeometry();
  ~GmReadPhantomGeometry();

  G4VPhysicalVolume* Construct();

protected:
  virtual void ReadPhantomData() = 0;

  virtual void ConstructPhantom(G4LogicalVolume* parentLV);
  
  std::pair<G4double,G4double> ReadVoxelDim( G4int nVoxel, std::ifstream& fin ); 
  G4Material* BuildMaterialChangingDensity( const G4Material* origMate, float density, G4String mateName );

  virtual void ReadVoxelDensities( std::ifstream& fin );
protected:
  G4LogicalVolume* world_logic;
  G4LogicalVolume* cont_logic;
  G4VPhysicalVolume* parameterisedPhysVolume; 

  G4int nVoxelX, nVoxelY, nVoxelZ;
  G4float dimX, dimY, dimZ;
  G4float offsetX, offsetY, offsetZ;

  std::vector<G4Material*> thePhantomMaterials; 
  G4Material* theMatePhantom;
  size_t* mateIDs;
  //t  size_t* mateIDs;

  G4ThreeVector theInitialDisp;
  G4double theInitialRotAngleX;
  G4double theInitialRotAngleY;
  G4double theInitialRotAngleZ;

  G4PhantomParameterisation* thePhantomParam;

  G4bool bRecalculateMaterialDensities;

  std::map<G4int,G4Material*> thePhantomMaterialsOriginal; // map numberOfMaterial to G4Material. They are the list of materials as built from .geom file

};

#endif
