#ifndef GmTraverseParallelPhysicalVolumeReplicatedFilter_hh
#define GmTraverseParallelPhysicalVolumeReplicatedFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionTraverseParallel.hh"
#include "GmTouchableFilterPhysicalVolumeReplicated.hh"
 
class GmTraverseParallelPhysicalVolumeReplicatedFilter : public GmVVolumeConditionFilter<GmGeomConditionTraverseParallel,GmTouchableFilterPhysicalVolumeReplicated>
{
public:
  GmTraverseParallelPhysicalVolumeReplicatedFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionTraverseParallel, GmTouchableFilterPhysicalVolumeReplicated>::GmVVolumeConditionFilter(name) {}
};

#endif
