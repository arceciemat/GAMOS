#ifndef GmStartPhysicalVolumeFilter_hh
#define GmStartPhysicalVolumeFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionStart.hh"
#include "GmTouchableFilterPhysicalVolume.hh"
 
class GmStartPhysicalVolumeFilter : public GmVVolumeConditionFilter<GmGeomConditionStart,GmTouchableFilterPhysicalVolume>
{
public:
  GmStartPhysicalVolumeFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionStart, GmTouchableFilterPhysicalVolume>::GmVVolumeConditionFilter(name) {}
};

#endif
