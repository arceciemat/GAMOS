#ifndef GmGeomUseMateDetectorBuilder_h
#define GmGeomUseMateDetectorBuilder_h
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
class GmGeomUseMateDetectorBuilder : public G4tgbDetectorBuilder
{ 
public:  
  GmGeomUseMateDetectorBuilder();
  ~GmGeomUseMateDetectorBuilder(){ };

  virtual const G4tgrVolume* ReadDetector();

private:
};

#endif

