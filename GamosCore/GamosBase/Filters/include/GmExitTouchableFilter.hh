#ifndef GmExitTouchableFilter_hh
#define GmExitTouchableFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionExit.hh"
#include "GmTouchableFilterTouchable.hh"
 
class GmExitTouchableFilter : public GmVVolumeConditionFilter<GmGeomConditionExit,GmTouchableFilterTouchable>
{
public:
  GmExitTouchableFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionExit, GmTouchableFilterTouchable>::GmVVolumeConditionFilter(name) {}
};

#endif
