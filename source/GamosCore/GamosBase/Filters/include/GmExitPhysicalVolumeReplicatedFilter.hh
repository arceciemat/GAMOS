#ifndef GmExitPhysicalVolumeReplicatedFilter_hh
#define GmExitPhysicalVolumeReplicatedFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionExit.hh"
#include "GmTouchableFilterPhysicalVolumeReplicated.hh"
 
class GmExitPhysicalVolumeReplicatedFilter : public GmVVolumeConditionFilter<GmGeomConditionExit,GmTouchableFilterPhysicalVolumeReplicated>
{
public:
  GmExitPhysicalVolumeReplicatedFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionExit, GmTouchableFilterPhysicalVolumeReplicated>::GmVVolumeConditionFilter(name) {}
};

#endif
