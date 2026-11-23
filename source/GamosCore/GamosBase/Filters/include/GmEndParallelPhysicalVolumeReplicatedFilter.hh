#ifndef GmEndParallelPhysicalVolumeReplicatedFilter_hh
#define GmEndParallelPhysicalVolumeReplicatedFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionEndParallel.hh"
#include "GmTouchableFilterPhysicalVolumeReplicated.hh"
 
class GmEndParallelPhysicalVolumeReplicatedFilter : public GmVVolumeConditionFilter<GmGeomConditionEndParallel,GmTouchableFilterPhysicalVolumeReplicated>
{
public:
  GmEndParallelPhysicalVolumeReplicatedFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEndParallel, GmTouchableFilterPhysicalVolumeReplicated>::GmVVolumeConditionFilter(name) {}
};

#endif
