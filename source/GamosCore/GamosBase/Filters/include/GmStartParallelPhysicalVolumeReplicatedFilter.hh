#ifndef GmStartParallelPhysicalVolumeReplicatedFilter_hh
#define GmStartParallelPhysicalVolumeReplicatedFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionStartParallel.hh"
#include "GmTouchableFilterPhysicalVolumeReplicated.hh"
 
class GmStartParallelPhysicalVolumeReplicatedFilter : public GmVVolumeConditionFilter<GmGeomConditionStartParallel,GmTouchableFilterPhysicalVolumeReplicated>
{
public:
  GmStartParallelPhysicalVolumeReplicatedFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionStartParallel, GmTouchableFilterPhysicalVolumeReplicated>::GmVVolumeConditionFilter(name) {}
};

#endif
