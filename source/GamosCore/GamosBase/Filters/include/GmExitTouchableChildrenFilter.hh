#ifndef GmExitTouchableChildrenFilter_hh
#define GmExitTouchableChildrenFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionExit.hh"
#include "GmTouchableFilterTouchableChildren.hh"
 
class GmExitTouchableChildrenFilter : public GmVVolumeConditionFilter<GmGeomConditionExit,GmTouchableFilterTouchableChildren>
{
public:
  GmExitTouchableChildrenFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionExit, GmTouchableFilterTouchableChildren>::GmVVolumeConditionFilter(name) {}
};

#endif
