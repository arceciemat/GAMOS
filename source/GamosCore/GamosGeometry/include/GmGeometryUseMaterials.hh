#ifndef GmGeometryUseMaterials_HH
#define GmGeometryUseMaterials_HH

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"

class GmGeometryUseMaterials :public G4VUserDetectorConstruction{
 public:
  GmGeometryUseMaterials();
  ~GmGeometryUseMaterials(){};

  G4VPhysicalVolume* Construct();
};

#endif
