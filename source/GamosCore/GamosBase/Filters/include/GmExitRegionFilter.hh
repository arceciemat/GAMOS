#ifndef GmExitRegionFilter_hh
#define GmExitRegionFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionExit.hh"
#include "GmTouchableFilterRegion.hh"
 
class GmExitRegionFilter : public GmVVolumeConditionFilter<GmGeomConditionExit,GmTouchableFilterRegion>
{
public:
  GmExitRegionFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionExit, GmTouchableFilterRegion>::GmVVolumeConditionFilter(name) {}
};

#endif
