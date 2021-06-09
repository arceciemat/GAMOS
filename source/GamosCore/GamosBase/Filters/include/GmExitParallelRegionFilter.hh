#ifndef GmExitParallelRegionFilter_hh
#define GmExitParallelRegionFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionExitParallel.hh"
#include "GmTouchableFilterRegion.hh"
 
class GmExitParallelRegionFilter : public GmVVolumeConditionFilter<GmGeomConditionExitParallel,GmTouchableFilterRegion>
{
public:
  GmExitParallelRegionFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionExitParallel, GmTouchableFilterRegion>::GmVVolumeConditionFilter(name) {}
};

#endif
