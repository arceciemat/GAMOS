#ifndef GmTraverseTouchableFilter_hh
#define GmTraverseTouchableFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionTraverse.hh"
#include "GmTouchableFilterTouchable.hh"
 
class GmTraverseTouchableFilter : public GmVVolumeConditionFilter<GmGeomConditionTraverse,GmTouchableFilterTouchable>
{
public:
  GmTraverseTouchableFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionTraverse, GmTouchableFilterTouchable>::GmVVolumeConditionFilter(name) {}
};

#endif
