#ifndef GmEndPhysicalVolumeChildrenReplicatedFilter_hh
#define GmEndPhysicalVolumeChildrenReplicatedFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionEnd.hh"
#include "GmTouchableFilterPhysicalVolumeReplicatedChildren.hh"
 
class GmEndPhysicalVolumeChildrenReplicatedFilter : public GmVVolumeConditionFilter<GmGeomConditionEnd,GmTouchableFilterPhysicalVolumeReplicatedChildren>
{
public:
  GmEndPhysicalVolumeChildrenReplicatedFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEnd, GmTouchableFilterPhysicalVolumeReplicatedChildren>::GmVVolumeConditionFilter(name) {}
};

#endif
