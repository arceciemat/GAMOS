#ifndef GmInParallelPhysicalVolumeFilter_hh
#define GmInParallelPhysicalVolumeFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionInParallel.hh"
#include "GmTouchableFilterPhysicalVolume.hh"
 
class GmInParallelPhysicalVolumeFilter : public GmVVolumeConditionFilter<GmGeomConditionInParallel,GmTouchableFilterPhysicalVolume>
{
public:
  GmInParallelPhysicalVolumeFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionInParallel, GmTouchableFilterPhysicalVolume>::GmVVolumeConditionFilter(name) {}
};

#endif
