#ifndef GmEndParallelLogicalVolumeFilter_hh
#define GmEndParallelLogicalVolumeFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionEndParallel.hh"
#include "GmTouchableFilterLogicalVolume.hh"
 
class GmEndParallelLogicalVolumeFilter : public GmVVolumeConditionFilter<GmGeomConditionEndParallel,GmTouchableFilterLogicalVolume>
{
public:
  GmEndParallelLogicalVolumeFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEndParallel, GmTouchableFilterLogicalVolume>::GmVVolumeConditionFilter(name) {}
};

#endif
