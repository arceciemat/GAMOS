#ifndef GmEnterRegionFilter_hh
#define GmEnterRegionFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionEnter.hh"
#include "GmTouchableFilterRegion.hh"
 
class GmEnterRegionFilter : public GmVVolumeConditionFilter<GmGeomConditionEnter,GmTouchableFilterRegion>
{
public:
  GmEnterRegionFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEnter, GmTouchableFilterRegion>::GmVVolumeConditionFilter(name) {}
};

#endif
