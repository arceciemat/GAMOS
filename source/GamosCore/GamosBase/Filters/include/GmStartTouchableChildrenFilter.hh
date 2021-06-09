#ifndef GmStartTouchableChildrenFilter_hh
#define GmStartTouchableChildrenFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionStart.hh"
#include "GmTouchableFilterTouchableChildren.hh"
 
class GmStartTouchableChildrenFilter : public GmVVolumeConditionFilter<GmGeomConditionStart,GmTouchableFilterTouchableChildren>
{
public:
  GmStartTouchableChildrenFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionStart, GmTouchableFilterTouchableChildren>::GmVVolumeConditionFilter(name) {}
};

#endif
