#ifndef GmInPhysicalVolumeReplicatedFilter_hh
#define GmInPhysicalVolumeReplicatedFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionIn.hh"
#include "GmTouchableFilterPhysicalVolumeReplicated.hh"
 
class GmInPhysicalVolumeReplicatedFilter : public GmVVolumeConditionFilter<GmGeomConditionIn,GmTouchableFilterPhysicalVolumeReplicated>
{
public:
  GmInPhysicalVolumeReplicatedFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionIn, GmTouchableFilterPhysicalVolumeReplicated>::GmVVolumeConditionFilter(name) {}
};

#endif
