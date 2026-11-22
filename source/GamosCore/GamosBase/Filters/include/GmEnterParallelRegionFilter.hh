#ifndef GmEnterParallelRegionFilter_hh
#define GmEnterParallelRegionFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionEnterParallel.hh"
#include "GmTouchableFilterRegion.hh"
 
class GmEnterParallelRegionFilter : public GmVVolumeConditionFilter<GmGeomConditionEnterParallel,GmTouchableFilterRegion>
{
public:
  GmEnterParallelRegionFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEnterParallel, GmTouchableFilterRegion>::GmVVolumeConditionFilter(name) {}
};

#endif
