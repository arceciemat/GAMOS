#ifndef GmStartPhysicalVolumeReplicatedFilter_hh
#define GmStartPhysicalVolumeReplicatedFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionStart.hh"
#include "GmTouchableFilterPhysicalVolumeReplicated.hh"
 
class GmStartPhysicalVolumeReplicatedFilter : public GmVVolumeConditionFilter<GmGeomConditionStart,GmTouchableFilterPhysicalVolumeReplicated>
{
public:
  GmStartPhysicalVolumeReplicatedFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionStart, GmTouchableFilterPhysicalVolumeReplicated>::GmVVolumeConditionFilter(name) {}
};

#endif
