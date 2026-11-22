#ifndef GmEnterTouchableChildrenFilter_hh
#define GmEnterTouchableChildrenFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionEnter.hh"
#include "GmTouchableFilterTouchableChildren.hh"
 
class GmEnterTouchableChildrenFilter : public GmVVolumeConditionFilter<GmGeomConditionEnter,GmTouchableFilterTouchableChildren>
{
public:
  GmEnterTouchableChildrenFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEnter, GmTouchableFilterTouchableChildren>::GmVVolumeConditionFilter(name) {}
};

#endif
