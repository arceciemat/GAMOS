#ifndef GmEndLogicalVolumeFilter_hh
#define GmEndLogicalVolumeFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionEnd.hh"
#include "GmTouchableFilterLogicalVolume.hh"
 
class GmEndLogicalVolumeFilter : public GmVVolumeConditionFilter<GmGeomConditionEnd,GmTouchableFilterLogicalVolume>
{
public:
  GmEndLogicalVolumeFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEnd, GmTouchableFilterLogicalVolume>::GmVVolumeConditionFilter(name) {}
};

#endif
