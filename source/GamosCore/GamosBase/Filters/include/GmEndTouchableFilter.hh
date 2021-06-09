#ifndef GmEndTouchableFilter_hh
#define GmEndTouchableFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionEnd.hh"
#include "GmTouchableFilterTouchable.hh"
 
class GmEndTouchableFilter : public GmVVolumeConditionFilter<GmGeomConditionEnd,GmTouchableFilterTouchable>
{
public:
  GmEndTouchableFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEnd, GmTouchableFilterTouchable>::GmVVolumeConditionFilter(name) {}
};

#endif
