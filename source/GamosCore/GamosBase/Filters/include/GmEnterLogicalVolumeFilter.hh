#ifndef GmEnterLogicalVolumeFilter_hh
#define GmEnterLogicalVolumeFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionEnter.hh"
#include "GmTouchableFilterLogicalVolume.hh"
 
class GmEnterLogicalVolumeFilter : public GmVVolumeConditionFilter<GmGeomConditionEnter,GmTouchableFilterLogicalVolume>
{
public:
  GmEnterLogicalVolumeFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEnter, GmTouchableFilterLogicalVolume>::GmVVolumeConditionFilter(name) {}
};

#endif
