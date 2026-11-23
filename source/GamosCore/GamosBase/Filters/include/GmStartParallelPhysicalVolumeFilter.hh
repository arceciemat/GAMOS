#ifndef GmStartParallelPhysicalVolumeFilter_hh
#define GmStartParallelPhysicalVolumeFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionStartParallel.hh"
#include "GmTouchableFilterPhysicalVolume.hh"
 
class GmStartParallelPhysicalVolumeFilter : public GmVVolumeConditionFilter<GmGeomConditionStartParallel,GmTouchableFilterPhysicalVolume>
{
public:
  GmStartParallelPhysicalVolumeFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionStartParallel, GmTouchableFilterPhysicalVolume>::GmVVolumeConditionFilter(name) {}
};

#endif
