#ifndef GmEndPhysicalVolumeFilter_hh
#define GmEndPhysicalVolumeFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionEnd.hh"
#include "GmTouchableFilterPhysicalVolume.hh"
 
class GmEndPhysicalVolumeFilter : public GmVVolumeConditionFilter<GmGeomConditionEnd,GmTouchableFilterPhysicalVolume>
{
public:
  GmEndPhysicalVolumeFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEnd, GmTouchableFilterPhysicalVolume>::GmVVolumeConditionFilter(name) {}
};

#endif
