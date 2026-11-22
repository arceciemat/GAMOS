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

  std::vector<G4Material*> GetPhantomMaterials() const {
    return thePhantomMaterials;
  }
  void SetPhantomMaterials( std::vector<G4Material*> pm ) {
    thePhantomMaterials = pm;
  }

  G4int GetNoVoxelsX() {
    return nVoxelX;
  }
  G4int GetNoVoxelsY() {
    return nVoxelY;
  }
    G4int GetNoVoxelsZ() {
    return nVoxelZ;
  }

  G4float GetMinX() const {
    return offsetX;
  }
  G4float GetMaxX() const {
    return offsetX+dimX*nVoxelX;
  }
  G4float GetMinY() const {
    return offsetY;
  }
  G4float GetMaxY() const {
    return offsetY+dimY*nVoxelY;
  }
  G4float GetMinZ() const {
    return offsetZ;
  }
  G4float GetMaxZ() const {
    return offsetZ+dimZ*nVoxelZ;
  }
  size_t* GetMateIDs() const {
    return theMateIDs;
  }
  float* GetMateDensities() const {
    return theMateDensities;
  }
  G4float GetVoxelHalfX() const {
    return dimX/2.;
  }
  G4float GetVoxelHalfY() const {
    return dimY/2.;
  }
  G4float GetVoxelHalfZ() const {
    return dimZ/2.;
  }
  G4float GetVoxelWidthX() const {
    return dimX;
  }
  G4float GetVoxelWidthY() const {
    return dimY;
  }
  G4float GetVoxelWidthZ() const {
    return dimZ;
  }

  G4int GetNoVoxels() {
    return nVoxelX * nVoxelY * nVoxelZ;
  }

  G4double GetInitialRotAngleX() const {
    return theInitialRotAngleX;
  }
  G4double GetInitialRotAngleY() const {
    return theInitialRotAngleY;
  }
  G4double GetInitialRotAngleZ() const {
    return theInitialRotAngleZ;
  }

  G4ThreeVector GetInitialDisp() const {
    return theInitialDisp;
  }

  void SetFileName( G4String fn ) {
    theFileName = fn;
  }
  void SetPhantomFileName( G4String fn ) {
    thePhantomFileName = fn;
  }
  void SetMotherName( G4String fn ) {
    theMotherName = fn;
  }

  void SetRecalculateMaterialDensities( G4bool brmd ) {
    bRecalculateMaterialDensities = brmd;
  }

protected:
  virtual void ReadPhantomData() = 0;

  virtual void ConstructPhantom(G4LogicalVolume* parentLV);
  
  std::pair<G4double,G4double> ReadVoxelDim( G4int nVoxel, std::ifstream& fin ); 
  G4Material* BuildMaterialChangingDensity( G4Material* origMate, float density, G4String mateName );

  virtual void ReadVoxelDensities( std::ifstream& fin );
protected:
  G4LogicalVolume* cont_logic;
  G4VPhysicalVolume* parameterisedPhysVolume; 

  G4int nVoxelX, nVoxelY, nVoxelZ;
  G4float dimX, dimY, dimZ;
  G4float offsetX, offsetY, offsetZ;

  std::vector<G4Material*> thePhantomMaterials; 
  G4Material* theMatePhantom;
  size_t* theMateIDs;
  float* theMateDensities;

  G4ThreeVector theInitialDisp;
  G4double theInitialRotAngleX;
  G4double theInitialRotAngleY;
  G4double theInitialRotAngleZ;

  G4PhantomParameterisation* thePhantomParam;

  G4bool bRecalculateMaterialDensities;

  std::map<G4int,G4Material*> thePhantomMaterialsOriginal; // map numberOfMaterial to G4Material. They are the list of materials as built from .geom file

  G4String theFileName;
  G4String thePhantomFileName;
  G4String theMotherName;
};

#endif
