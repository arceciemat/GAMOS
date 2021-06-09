#ifndef GmInParallelPhysicalVolumeReplicatedFilter_hh
#define GmInParallelPhysicalVolumeReplicatedFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionInParallel.hh"
#include "GmTouchableFilterPhysicalVolumeReplicated.hh"
 
class GmInParallelPhysicalVolumeReplicatedFilter : public GmVVolumeConditionFilter<GmGeomConditionInParallel,GmTouchableFilterPhysicalVolumeReplicated>
{
public:
  GmInParallelPhysicalVolumeReplicatedFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionInParallel, GmTouchableFilterPhysicalVolumeReplicated>::GmVVolumeConditionFilter(name) {}
};

#endif
