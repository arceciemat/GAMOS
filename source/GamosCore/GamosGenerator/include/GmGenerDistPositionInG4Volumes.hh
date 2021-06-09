#ifndef GmGenerDistPositionInG4Volumes_HH
#define GmGenerDistPositionInG4Volumes_HH

#include "GmVGenerDistPositionVolumesAndSurfaces.hh"
#include "GmPositionG4Volumes.hh"
#include "GmPositionVolumePos.hh"

class GmGenerDistPositionInG4Volumes : public GmVGenerDistPositionVolumesAndSurfaces
{
public:
  GmGenerDistPositionInG4Volumes(){
    theOrigin= new GmPositionG4Volumes;
    theObject = new GmPositionVolumePos;
}
  ~GmGenerDistPositionInG4Volumes(){}
};

#endif
