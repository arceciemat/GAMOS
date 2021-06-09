#ifndef GmEndPhysicalVolumeReplicatedFilter_hh
#define GmEndPhysicalVolumeReplicatedFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionEnd.hh"
#include "GmTouchableFilterPhysicalVolumeReplicated.hh"
 
class GmEndPhysicalVolumeReplicatedFilter : public GmVVolumeConditionFilter<GmGeomConditionEnd,GmTouchableFilterPhysicalVolumeReplicated>
{
public:
  GmEndPhysicalVolumeReplicatedFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEnd, GmTouchableFilterPhysicalVolumeReplicated>::GmVVolumeConditionFilter(name) {}
};

#endif
