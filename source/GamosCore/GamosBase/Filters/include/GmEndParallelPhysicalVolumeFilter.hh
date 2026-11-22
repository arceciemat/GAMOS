#ifndef GmEndParallelPhysicalVolumeFilter_hh
#define GmEndParallelPhysicalVolumeFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionEndParallel.hh"
#include "GmTouchableFilterPhysicalVolume.hh"
 
class GmEndParallelPhysicalVolumeFilter : public GmVVolumeConditionFilter<GmGeomConditionEndParallel,GmTouchableFilterPhysicalVolume>
{
public:
  GmEndParallelPhysicalVolumeFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEndParallel, GmTouchableFilterPhysicalVolume>::GmVVolumeConditionFilter(name) {}
};

#endif
