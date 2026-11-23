#ifndef GmExitPhysicalVolumeChildrenReplicatedFilter_hh
#define GmExitPhysicalVolumeChildrenReplicatedFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionExit.hh"
#include "GmTouchableFilterPhysicalVolumeReplicatedChildren.hh"
 
class GmExitPhysicalVolumeChildrenReplicatedFilter : public GmVVolumeConditionFilter<GmGeomConditionExit,GmTouchableFilterPhysicalVolumeReplicatedChildren>
{
public:
  GmExitPhysicalVolumeChildrenReplicatedFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionExit, GmTouchableFilterPhysicalVolumeReplicatedChildren>::GmVVolumeConditionFilter(name) {}
};

#endif
