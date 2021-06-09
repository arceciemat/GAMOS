#ifndef GmEnterPhysicalVolumeReplicatedFilter_hh
#define GmEnterPhysicalVolumeReplicatedFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionEnter.hh"
#include "GmTouchableFilterPhysicalVolumeReplicated.hh"
 
class GmEnterPhysicalVolumeReplicatedFilter : public GmVVolumeConditionFilter<GmGeomConditionEnter,GmTouchableFilterPhysicalVolumeReplicated>
{
public:
  GmEnterPhysicalVolumeReplicatedFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEnter, GmTouchableFilterPhysicalVolumeReplicated>::GmVVolumeConditionFilter(name) {}
};

#endif
