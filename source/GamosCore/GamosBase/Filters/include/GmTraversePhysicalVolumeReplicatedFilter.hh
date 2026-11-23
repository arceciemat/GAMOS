#ifndef GmTraversePhysicalVolumeReplicatedFilter_hh
#define GmTraversePhysicalVolumeReplicatedFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionTraverse.hh"
#include "GmTouchableFilterPhysicalVolumeReplicated.hh"
 
class GmTraversePhysicalVolumeReplicatedFilter : public GmVVolumeConditionFilter<GmGeomConditionTraverse,GmTouchableFilterPhysicalVolumeReplicated>
{
public:
  GmTraversePhysicalVolumeReplicatedFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionTraverse, GmTouchableFilterPhysicalVolumeReplicated>::GmVVolumeConditionFilter(name) {}
};

#endif
