#ifndef GmStartTouchableFilter_hh
#define GmStartTouchableFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionStart.hh"
#include "GmTouchableFilterTouchable.hh"
 
class GmStartTouchableFilter : public GmVVolumeConditionFilter<GmGeomConditionStart,GmTouchableFilterTouchable>
{
public:
  GmStartTouchableFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionStart, GmTouchableFilterTouchable>::GmVVolumeConditionFilter(name) {}
};

#endif
