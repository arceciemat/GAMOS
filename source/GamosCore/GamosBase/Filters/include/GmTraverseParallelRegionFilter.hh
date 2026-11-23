#ifndef GmTraverseParallelRegionFilter_hh
#define GmTraverseParallelRegionFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionTraverseParallel.hh"
#include "GmTouchableFilterRegion.hh"
 
class GmTraverseParallelRegionFilter : public GmVVolumeConditionFilter<GmGeomConditionTraverseParallel,GmTouchableFilterRegion>
{
public:
  GmTraverseParallelRegionFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionTraverseParallel, GmTouchableFilterRegion>::GmVVolumeConditionFilter(name) {}
};

#endif
