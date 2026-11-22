#ifndef DicomPolygon__HH
#define DicomPolygon__HH

#include "DicomVLine.hh"
#include "G4ThreeVector.hh"
#include <vector>
class DicomVImageStr;
class G4ExtrudedSolid;

class DicomPolygon : public DicomVLine
{ 
public:
  DicomPolygon( G4String name, DPOrientation ori);
  DicomPolygon( std::vector<G4ThreeVector> points, std::vector<G4ThreeVector> dirs, G4String name, DPOrientation ori);
  DicomPolygon( std::vector<G4ThreeVector> points, G4String name, DPOrientation ori);
  
  ~DicomPolygon(){};
  void FindVoxelsInXY( DicomVImageStr* image, G4double extraZ = 0. );
  G4int GetPolygonZIndex( DicomVImage* image, G4double extraZ = 0. );
  G4bool PointIsInXY( G4ThreeVector point);
  
  G4ExtrudedSolid* BuildExtrudedSolid(G4double dimZ);

  void SetPoints( std::vector<G4ThreeVector> points );
  void BuildDirections();

};

#endif
  
