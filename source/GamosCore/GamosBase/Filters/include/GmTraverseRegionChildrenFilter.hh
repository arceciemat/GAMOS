#ifndef GmTraverseRegionChildrenFilter_hh
#define GmTraverseRegionChildrenFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionTraverse.hh"
#include "GmTouchableFilterRegionChildren.hh"
 
class GmTraverseRegionChildrenFilter : public GmVVolumeConditionFilter<GmGeomConditionTraverse,GmTouchableFilterRegionChildren>
{
public:
  GmTraverseRegionChildrenFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionTraverse, GmTouchableFilterRegionChildren>::GmVVolumeConditionFilter(name) {}
};

#endif
