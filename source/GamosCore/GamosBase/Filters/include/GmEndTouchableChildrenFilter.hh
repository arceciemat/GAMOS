#ifndef GmEndTouchableChildrenFilter_hh
#define GmEndTouchableChildrenFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionEnd.hh"
#include "GmTouchableFilterTouchableChildren.hh"
 
class GmEndTouchableChildrenFilter : public GmVVolumeConditionFilter<GmGeomConditionEnd,GmTouchableFilterTouchableChildren>
{
public:
  GmEndTouchableChildrenFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEnd, GmTouchableFilterTouchableChildren>::GmVVolumeConditionFilter(name) {}
};

#endif
