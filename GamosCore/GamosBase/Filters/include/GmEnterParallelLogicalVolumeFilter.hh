#ifndef GmEnterParallelLogicalVolumeFilter_hh
#define GmEnterParallelLogicalVolumeFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionEnterParallel.hh"
#include "GmTouchableFilterLogicalVolume.hh"
 
class GmEnterParallelLogicalVolumeFilter : public GmVVolumeConditionFilter<GmGeomConditionEnterParallel,GmTouchableFilterLogicalVolume>
{
public:
  GmEnterParallelLogicalVolumeFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEnterParallel, GmTouchableFilterLogicalVolume>::GmVVolumeConditionFilter(name) {}
};

#endif
