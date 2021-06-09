#ifndef GmInPhysicalVolumeChildrenReplicatedFilter_hh
#define GmInPhysicalVolumeChildrenReplicatedFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionIn.hh"
#include "GmTouchableFilterPhysicalVolumeReplicatedChildren.hh"
 
class GmInPhysicalVolumeChildrenReplicatedFilter : public GmVVolumeConditionFilter<GmGeomConditionIn,GmTouchableFilterPhysicalVolumeReplicatedChildren>
{
public:
  GmInPhysicalVolumeChildrenReplicatedFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionIn, GmTouchableFilterPhysicalVolumeReplicatedChildren>::GmVVolumeConditionFilter(name) {}
};

#endif
