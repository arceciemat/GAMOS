#ifndef GmInTouchableFilter_hh
#define GmInTouchableFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionIn.hh"
#include "GmTouchableFilterTouchable.hh"
 
class GmInTouchableFilter : public GmVVolumeConditionFilter<GmGeomConditionIn,GmTouchableFilterTouchable>
{
public:
  GmInTouchableFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionIn, GmTouchableFilterTouchable>::GmVVolumeConditionFilter(name) {}
};

#endif
