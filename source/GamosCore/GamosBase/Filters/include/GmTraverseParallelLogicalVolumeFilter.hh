#ifndef GmTraverseParallelLogicalVolumeFilter_hh
#define GmTraverseParallelLogicalVolumeFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionTraverseParallel.hh"
#include "GmTouchableFilterLogicalVolume.hh"
 
class GmTraverseParallelLogicalVolumeFilter : public GmVVolumeConditionFilter<GmGeomConditionTraverseParallel,GmTouchableFilterLogicalVolume>
{
public:
  GmTraverseParallelLogicalVolumeFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionTraverseParallel, GmTouchableFilterLogicalVolume>::GmVVolumeConditionFilter(name) {}
};

#endif
