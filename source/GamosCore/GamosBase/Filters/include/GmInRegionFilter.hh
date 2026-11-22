#ifndef GmInRegionFilter_hh
#define GmInRegionFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionIn.hh"
#include "GmTouchableFilterRegion.hh"
 
class GmInRegionFilter : public GmVVolumeConditionFilter<GmGeomConditionIn,GmTouchableFilterRegion>
{
public:
  GmInRegionFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionIn, GmTouchableFilterRegion>::GmVVolumeConditionFilter(name) {}
};

#endif
