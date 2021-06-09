#ifndef GmInParallelRegionFilter_hh
#define GmInParallelRegionFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionInParallel.hh"
#include "GmTouchableFilterRegion.hh"
 
class GmInParallelRegionFilter : public GmVVolumeConditionFilter<GmGeomConditionInParallel,GmTouchableFilterRegion>
{
public:
  GmInParallelRegionFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionInParallel, GmTouchableFilterRegion>::GmVVolumeConditionFilter(name) {}
};

#endif
