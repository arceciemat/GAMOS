#ifndef GmStartPhysicalVolumeChildrenReplicatedFilter_hh
#define GmStartPhysicalVolumeChildrenReplicatedFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionStart.hh"
#include "GmTouchableFilterPhysicalVolumeReplicatedChildren.hh"
 
class GmStartPhysicalVolumeChildrenReplicatedFilter : public GmVVolumeConditionFilter<GmGeomConditionStart,GmTouchableFilterPhysicalVolumeReplicatedChildren>
{
public:
  GmStartPhysicalVolumeChildrenReplicatedFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionStart, GmTouchableFilterPhysicalVolumeReplicatedChildren>::GmVVolumeConditionFilter(name) {}
};

#endif
