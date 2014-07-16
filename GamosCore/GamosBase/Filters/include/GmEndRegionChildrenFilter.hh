#ifndef GmEndRegionChildrenFilter_hh
#define GmEndRegionChildrenFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionEnd.hh"
#include "GmTouchableFilterRegionChildren.hh"
 
class GmEndRegionChildrenFilter : public GmVVolumeConditionFilter<GmGeomConditionEnd,GmTouchableFilterRegionChildren>
{
public:
  GmEndRegionChildrenFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEnd, GmTouchableFilterRegionChildren>::GmVVolumeConditionFilter(name) {}
};

#endif
