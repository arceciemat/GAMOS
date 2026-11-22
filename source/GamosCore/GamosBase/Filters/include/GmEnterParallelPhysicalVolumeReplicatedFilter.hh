#ifndef GmEnterParallelPhysicalVolumeReplicatedFilter_hh
#define GmEnterParallelPhysicalVolumeReplicatedFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionEnterParallel.hh"
#include "GmTouchableFilterPhysicalVolumeReplicated.hh"
 
class GmEnterParallelPhysicalVolumeReplicatedFilter : public GmVVolumeConditionFilter<GmGeomConditionEnterParallel,GmTouchableFilterPhysicalVolumeReplicated>
{
public:
  GmEnterParallelPhysicalVolumeReplicatedFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEnterParallel, GmTouchableFilterPhysicalVolumeReplicated>::GmVVolumeConditionFilter(name) {}
};

#endif
