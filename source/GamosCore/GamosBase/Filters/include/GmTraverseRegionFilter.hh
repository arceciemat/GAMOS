#ifndef GmTraverseRegionFilter_hh
#define GmTraverseRegionFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionTraverse.hh"
#include "GmTouchableFilterRegion.hh"
 
class GmTraverseRegionFilter : public GmVVolumeConditionFilter<GmGeomConditionTraverse,GmTouchableFilterRegion>
{
public:
  GmTraverseRegionFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionTraverse, GmTouchableFilterRegion>::GmVVolumeConditionFilter(name) {}
};

#endif
