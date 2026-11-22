#ifndef DicomOperExtendAir__HH
#define DicomOperExtendAir__HH

#include "DicomVOperator.hh"
class DicomVImage;
class DicomVImageStr;

class DicomOperExtendAir : public DicomVOperator
{ 
public:
  DicomOperExtendAir( G4int iOrder = INT_MAX, G4String name = "DicomOperExtendAir");
  ~DicomOperExtendAir(){};

public:
  virtual void Operate( DicomVImage* );
  void Operate( DicomVImage* image, G4int noVoxelX, G4double minX, G4double maxX, G4int noVoxelY, G4double minY, G4double maxY, G4int noVoxelZ, G4double minZ, G4double maxZ);
  
private:
  void OperateStr( DicomVImageStr* imageStr, G4int newNoVoxelsX, G4double newMinX, G4double newMaxX, G4int newNoVoxelsY, G4double newMinY, G4double newMaxY, G4int newNoVoxelsZ, G4double newMinZ, G4double newMaxZ);
};

#endif
  
