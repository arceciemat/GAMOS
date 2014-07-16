#ifndef GmInParallelLogicalVolumeFilter_hh
#define GmInParallelLogicalVolumeFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionInParallel.hh"
#include "GmTouchableFilterLogicalVolume.hh"
 
class GmInParallelLogicalVolumeFilter : public GmVVolumeConditionFilter<GmGeomConditionInParallel,GmTouchableFilterLogicalVolume>
{
public:
  GmInParallelLogicalVolumeFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionInParallel, GmTouchableFilterLogicalVolume>::GmVVolumeConditionFilter(name) {}
};

#endif
