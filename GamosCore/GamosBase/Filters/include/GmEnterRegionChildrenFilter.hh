#ifndef GmEnterRegionChildrenFilter_hh
#define GmEnterRegionChildrenFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionEnter.hh"
#include "GmTouchableFilterRegionChildren.hh"
 
class GmEnterRegionChildrenFilter : public GmVVolumeConditionFilter<GmGeomConditionEnter,GmTouchableFilterRegionChildren>
{
public:
  GmEnterRegionChildrenFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEnter, GmTouchableFilterRegionChildren>::GmVVolumeConditionFilter(name) {}
};

#endif
