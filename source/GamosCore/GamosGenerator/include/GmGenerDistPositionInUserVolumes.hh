#ifndef GmGenerDistPositionInUserVolumes_HH
#define GmGenerDistPositionInUserVolumes_HH

#include "GmVGenerDistPositionVolumesAndSurfaces.hh"
#include "GmPositionUserVolumes.hh"
#include "GmPositionVolumePos.hh"

class GmGenerDistPositionInUserVolumes : public GmVGenerDistPositionVolumesAndSurfaces
{
public:
  GmGenerDistPositionInUserVolumes(){
    theOrigin= new GmPositionUserVolumes;
    theObject = new GmPositionVolumePos;
  }
  ~GmGenerDistPositionInUserVolumes(){}

};

#endif
