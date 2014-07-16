#ifndef GmGeomTextDetectorBuilder_h
#define GmGeomTextDetectorBuilder_h
#include "globals.hh"

/*---------------------------------------------------------------------------   
Author:      P. Arce
Changes:     creation   May 2007
---------------------------------------------------------------------------*/ 
// Description  
//----------------------------------------------- 
/*!  */ 


#include "G4tgbDetectorBuilder.hh"

//---------------------------------------------------------------------------- 
class GmGeomTextDetectorBuilder : public G4tgbDetectorBuilder
{ 
public:  
  GmGeomTextDetectorBuilder();
  ~GmGeomTextDetectorBuilder(){ };

  virtual const G4tgrVolume* ReadDetector();
  G4VPhysicalVolume* ConstructDetectorGAMOS( const G4tgrVolume* tgrVoltop, G4int parallelID = -1, G4bool bBuildRegionCuts = 1 );

private:
};

#endif

