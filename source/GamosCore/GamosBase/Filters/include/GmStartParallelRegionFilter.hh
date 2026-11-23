#ifndef GmStartParallelRegionFilter_hh
#define GmStartParallelRegionFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionStartParallel.hh"
#include "GmTouchableFilterRegion.hh"
 
class GmStartParallelRegionFilter : public GmVVolumeConditionFilter<GmGeomConditionStartParallel,GmTouchableFilterRegion>
{
public:
  GmStartParallelRegionFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionStartParallel, GmTouchableFilterRegion>::GmVVolumeConditionFilter(name) {}
};

#endif
