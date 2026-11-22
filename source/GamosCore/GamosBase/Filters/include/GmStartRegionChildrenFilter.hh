#ifndef GmStartRegionChildrenFilter_hh
#define GmStartRegionChildrenFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionStart.hh"
#include "GmTouchableFilterRegionChildren.hh"
 
class GmStartRegionChildrenFilter : public GmVVolumeConditionFilter<GmGeomConditionStart,GmTouchableFilterRegionChildren>
{
public:
  GmStartRegionChildrenFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionStart, GmTouchableFilterRegionChildren>::GmVVolumeConditionFilter(name) {}
};

#endif
