#ifndef GmGeometryFromText_HH
#define GmGeometryFromText_HH

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"

class GmGeometryFromText :public G4VUserDetectorConstruction{
 public:
  GmGeometryFromText();
  ~GmGeometryFromText(){};

  G4VPhysicalVolume* Construct();

protected:  
  G4String theClassName;
  G4VPhysicalVolume* thePhysiWorld;
};

#endif
