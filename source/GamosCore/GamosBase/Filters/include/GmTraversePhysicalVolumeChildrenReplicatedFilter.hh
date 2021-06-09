#ifndef GmTraversePhysicalVolumeChildrenReplicatedFilter_hh
#define GmTraversePhysicalVolumeChildrenReplicatedFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionTraverse.hh"
#include "GmTouchableFilterPhysicalVolumeReplicatedChildren.hh"
 
class GmTraversePhysicalVolumeChildrenReplicatedFilter : public GmVVolumeConditionFilter<GmGeomConditionTraverse,GmTouchableFilterPhysicalVolumeReplicatedChildren>
{
public:
  GmTraversePhysicalVolumeChildrenReplicatedFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionTraverse, GmTouchableFilterPhysicalVolumeReplicatedChildren>::GmVVolumeConditionFilter(name) {}
};

#endif
