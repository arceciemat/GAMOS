
#ifndef GmReadNMDicom_h
#define GmReadNMDicom_h 1

#include "globals.hh"
#include "GamosCore/GamosUtils/include/GmFileIn.hh"

class G4Material;
class G4LogicalVolume;
class G4PhysicalVolume;
class G4Box;


class GmReadNMDicom 
{
public:

  GmReadNMDicom();
  ~GmReadNMDicom();

public:
  virtual void ReadData( std::string fileName = "" );
  
  G4double* GetActivities() const {
    return theActivities;
  }
  G4double GetPixel(int ii) const {
    return theActivities[ii];
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
  G4float GetVoxelHalfX() const {
    return dimX/2.;
  }
  G4float GetVoxelHalfY() const {
    return dimY/2.;
  }
  G4float GetVoxelHalfZ() const {
    return dimZ/2.;
  }

  G4int GetNoVoxels() {
    return nVoxelX * nVoxelY * nVoxelZ;
  }
  
private:
  G4int nVoxelX, nVoxelY, nVoxelZ;
  G4float dimX, dimY, dimZ;
  G4float offsetX, offsetY, offsetZ;

  G4double* theActivities;
  
  //-  std::pair<G4double,G4double> ReadVoxelDim( G4int nVoxel, std::ifstream& fin ); 
};

#endif
