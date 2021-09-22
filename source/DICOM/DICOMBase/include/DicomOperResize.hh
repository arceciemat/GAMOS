#ifndef DicomOperResize__HH
#define DicomOperResize__HH

#include "DicomVOperator.hh"
class DicomVImage;

class DicomOperResize : public DicomVOperator
{ 
public:
  DicomOperResize( G4int iOrder = INT_MAX, G4String name = "DicomOperResize");
  ~DicomOperResize(){};

public:
  virtual void Operate( DicomVImage* );
  void Operate( DicomVImage* image, G4int noVoxelX, G4double minX, G4double maxX, G4int noVoxelY, G4double minY, G4double maxY, G4int noVoxelZ, G4double minZ, G4double maxZ);
  
private:
};

#endif
  
