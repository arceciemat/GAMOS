#ifndef GmStartRegionFilter_hh
#define GmStartRegionFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionStart.hh"
#include "GmTouchableFilterRegion.hh"
 
class GmStartRegionFilter : public GmVVolumeConditionFilter<GmGeomConditionStart,GmTouchableFilterRegion>
{
public:
  GmStartRegionFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionStart, GmTouchableFilterRegion>::GmVVolumeConditionFilter(name) {}
};

#endif
