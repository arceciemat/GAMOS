#ifndef GmExitRegionChildrenFilter_hh
#define GmExitRegionChildrenFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionExit.hh"
#include "GmTouchableFilterRegionChildren.hh"
 
class GmExitRegionChildrenFilter : public GmVVolumeConditionFilter<GmGeomConditionExit,GmTouchableFilterRegionChildren>
{
public:
  GmExitRegionChildrenFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionExit, GmTouchableFilterRegionChildren>::GmVVolumeConditionFilter(name) {}
};

#endif
