#ifndef GmGenerDistPositionInG4Materials_HH
#define GmGenerDistPositionInG4Materials_HH

#include "GmVGenerDistPositionVolumesAndSurfaces.hh"
#include "GmPositionG4Materials.hh"
#include "GmPositionVolumePos.hh"

class GmGenerDistPositionInG4Materials : public GmVGenerDistPositionVolumesAndSurfaces
{
public:
  GmGenerDistPositionInG4Materials(){
    theOrigin= new GmPositionG4Materials;
    theObject = new GmPositionVolumePos;
}
  ~GmGenerDistPositionInG4Materials(){}
};

#endif
