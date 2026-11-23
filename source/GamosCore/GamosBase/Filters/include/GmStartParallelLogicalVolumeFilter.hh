#ifndef GmStartParallelLogicalVolumeFilter_hh
#define GmStartParallelLogicalVolumeFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionStartParallel.hh"
#include "GmTouchableFilterLogicalVolume.hh"
 
class GmStartParallelLogicalVolumeFilter : public GmVVolumeConditionFilter<GmGeomConditionStartParallel,GmTouchableFilterLogicalVolume>
{
public:
  GmStartParallelLogicalVolumeFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionStartParallel, GmTouchableFilterLogicalVolume>::GmVVolumeConditionFilter(name) {}
};

#endif
