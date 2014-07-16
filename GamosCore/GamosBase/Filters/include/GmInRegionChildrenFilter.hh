#ifndef GmInRegionChildrenFilter_hh
#define GmInRegionChildrenFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionIn.hh"
#include "GmTouchableFilterRegionChildren.hh"
 
class GmInRegionChildrenFilter : public GmVVolumeConditionFilter<GmGeomConditionIn,GmTouchableFilterRegionChildren>
{
public:
  GmInRegionChildrenFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionIn, GmTouchableFilterRegionChildren>::GmVVolumeConditionFilter(name) {}
};

#endif
