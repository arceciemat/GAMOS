#ifndef GmGenerDistPositionInUserSurfaces_HH
#define GmGenerDistPositionInUserSurfaces_HH

#include "GmVGenerDistPositionVolumesAndSurfaces.hh"
#include "GmPositionUserVolumes.hh"
#include "GmPositionSurfacePos.hh"

class GmGenerDistPositionInUserSurfaces : public GmVGenerDistPositionVolumesAndSurfaces
{
public:
  GmGenerDistPositionInUserSurfaces(){
    theOrigin= new GmPositionUserVolumes;
    theObject = new GmPositionSurfacePos;
  }
  ~GmGenerDistPositionInUserSurfaces(){}

};

#endif
