#ifndef GmEnterPhysicalVolumeFilter_hh
#define GmEnterPhysicalVolumeFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionEnter.hh"
#include "GmTouchableFilterPhysicalVolume.hh"
 
class GmEnterPhysicalVolumeFilter : public GmVVolumeConditionFilter<GmGeomConditionEnter,GmTouchableFilterPhysicalVolume>
{
public:
  GmEnterPhysicalVolumeFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEnter, GmTouchableFilterPhysicalVolume>::GmVVolumeConditionFilter(name) {}
};

#endif
