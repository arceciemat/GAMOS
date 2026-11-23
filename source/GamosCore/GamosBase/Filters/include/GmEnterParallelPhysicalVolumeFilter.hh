#ifndef GmEnterParallelPhysicalVolumeFilter_hh
#define GmEnterParallelPhysicalVolumeFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionEnterParallel.hh"
#include "GmTouchableFilterPhysicalVolume.hh"
 
class GmEnterParallelPhysicalVolumeFilter : public GmVVolumeConditionFilter<GmGeomConditionEnterParallel,GmTouchableFilterPhysicalVolume>
{
public:
  GmEnterParallelPhysicalVolumeFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEnterParallel, GmTouchableFilterPhysicalVolume>::GmVVolumeConditionFilter(name) {}
};

#endif
