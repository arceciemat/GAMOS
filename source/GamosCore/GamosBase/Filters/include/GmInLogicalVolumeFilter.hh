#ifndef GmInLogicalVolumeFilter_hh
#define GmInLogicalVolumeFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionIn.hh"
#include "GmTouchableFilterLogicalVolume.hh"
 
class GmInLogicalVolumeFilter : public GmVVolumeConditionFilter<GmGeomConditionIn,GmTouchableFilterLogicalVolume>
{
public:
  GmInLogicalVolumeFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionIn, GmTouchableFilterLogicalVolume>::GmVVolumeConditionFilter(name) {}
};

#endif
