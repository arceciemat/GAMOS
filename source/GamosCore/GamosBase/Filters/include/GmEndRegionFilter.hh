#ifndef GmEndRegionFilter_hh
#define GmEndRegionFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionEnd.hh"
#include "GmTouchableFilterRegion.hh"
 
class GmEndRegionFilter : public GmVVolumeConditionFilter<GmGeomConditionEnd,GmTouchableFilterRegion>
{
public:
  GmEndRegionFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEnd, GmTouchableFilterRegion>::GmVVolumeConditionFilter(name) {}
};

#endif
