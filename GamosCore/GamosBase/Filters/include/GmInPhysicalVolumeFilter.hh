#ifndef GmInPhysicalVolumeFilter_hh
#define GmInPhysicalVolumeFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionIn.hh"
#include "GmTouchableFilterPhysicalVolume.hh"
 
class GmInPhysicalVolumeFilter : public GmVVolumeConditionFilter<GmGeomConditionIn,GmTouchableFilterPhysicalVolume>
{
public:
  GmInPhysicalVolumeFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionIn, GmTouchableFilterPhysicalVolume>::GmVVolumeConditionFilter(name) {}
};

#endif
