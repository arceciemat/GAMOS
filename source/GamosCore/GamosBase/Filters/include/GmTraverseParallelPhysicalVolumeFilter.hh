#ifndef GmTraverseParallelPhysicalVolumeFilter_hh
#define GmTraverseParallelPhysicalVolumeFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionTraverseParallel.hh"
#include "GmTouchableFilterPhysicalVolume.hh"
 
class GmTraverseParallelPhysicalVolumeFilter : public GmVVolumeConditionFilter<GmGeomConditionTraverseParallel,GmTouchableFilterPhysicalVolume>
{
public:
  GmTraverseParallelPhysicalVolumeFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionTraverseParallel, GmTouchableFilterPhysicalVolume>::GmVVolumeConditionFilter(name) {}
};

#endif
