#ifndef GmExitParallelPhysicalVolumeReplicatedFilter_hh
#define GmExitParallelPhysicalVolumeReplicatedFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionExitParallel.hh"
#include "GmTouchableFilterPhysicalVolumeReplicated.hh"
 
class GmExitParallelPhysicalVolumeReplicatedFilter : public GmVVolumeConditionFilter<GmGeomConditionExitParallel,GmTouchableFilterPhysicalVolumeReplicated>
{
public:
  GmExitParallelPhysicalVolumeReplicatedFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionExitParallel, GmTouchableFilterPhysicalVolumeReplicated>::GmVVolumeConditionFilter(name) {}
};

#endif
