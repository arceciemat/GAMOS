#ifndef GmInTouchableChildrenFilter_hh
#define GmInTouchableChildrenFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionIn.hh"
#include "GmTouchableFilterTouchableChildren.hh"
 
class GmInTouchableChildrenFilter : public GmVVolumeConditionFilter<GmGeomConditionIn,GmTouchableFilterTouchableChildren>
{
public:
  GmInTouchableChildrenFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionIn, GmTouchableFilterTouchableChildren>::GmVVolumeConditionFilter(name) {}
};

#endif
