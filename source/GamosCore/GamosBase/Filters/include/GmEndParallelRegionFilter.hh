#ifndef GmEndParallelRegionFilter_hh
#define GmEndParallelRegionFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionEndParallel.hh"
#include "GmTouchableFilterRegion.hh"
 
class GmEndParallelRegionFilter : public GmVVolumeConditionFilter<GmGeomConditionEndParallel,GmTouchableFilterRegion>
{
public:
  GmEndParallelRegionFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEndParallel, GmTouchableFilterRegion>::GmVVolumeConditionFilter(name) {}
};

#endif
