#ifndef GmGenerDistPositionInG4Surfaces_HH
#define GmGenerDistPositionInG4Surfaces_HH

#include "GmVGenerDistPositionVolumesAndSurfaces.hh"
#include "GmPositionG4Volumes.hh"
#include "GmPositionSurfacePos.hh"

class GmGenerDistPositionInG4Surfaces : public GmVGenerDistPositionVolumesAndSurfaces
{
public:
  GmGenerDistPositionInG4Surfaces(){ 
    theOrigin= new GmPositionG4Volumes;
    theObject = new GmPositionSurfacePos;
  }
  ~GmGenerDistPositionInG4Surfaces(){}

};

#endif
