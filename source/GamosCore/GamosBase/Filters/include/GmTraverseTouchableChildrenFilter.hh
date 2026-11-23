#ifndef GmTraverseTouchableChildrenFilter_hh
#define GmTraverseTouchableChildrenFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionTraverse.hh"
#include "GmTouchableFilterTouchableChildren.hh"
 
class GmTraverseTouchableChildrenFilter : public GmVVolumeConditionFilter<GmGeomConditionTraverse,GmTouchableFilterTouchableChildren>
{
public:
  GmTraverseTouchableChildrenFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionTraverse, GmTouchableFilterTouchableChildren>::GmVVolumeConditionFilter(name) {}
};

#endif
