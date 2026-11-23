#ifndef GmEnterTouchableFilter_hh
#define GmEnterTouchableFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionEnter.hh"
#include "GmTouchableFilterTouchable.hh"
 
class GmEnterTouchableFilter : public GmVVolumeConditionFilter<GmGeomConditionEnter,GmTouchableFilterTouchable>
{
public:
  GmEnterTouchableFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEnter, GmTouchableFilterTouchable>::GmVVolumeConditionFilter(name) {}
};

#endif
