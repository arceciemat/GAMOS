#ifndef GmEnterPhysicalVolumeChildrenReplicatedFilter_hh
#define GmEnterPhysicalVolumeChildrenReplicatedFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionEnter.hh"
#include "GmTouchableFilterPhysicalVolumeReplicatedChildren.hh"
 
class GmEnterPhysicalVolumeChildrenReplicatedFilter : public GmVVolumeConditionFilter<GmGeomConditionEnter,GmTouchableFilterPhysicalVolumeReplicatedChildren>
{
public:
  GmEnterPhysicalVolumeChildrenReplicatedFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEnter, GmTouchableFilterPhysicalVolumeReplicatedChildren>::GmVVolumeConditionFilter(name) {}
};

#endif
