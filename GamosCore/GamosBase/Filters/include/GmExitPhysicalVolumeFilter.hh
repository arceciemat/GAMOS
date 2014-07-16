#ifndef GmExitPhysicalVolumeFilter_hh
#define GmExitPhysicalVolumeFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionExit.hh"
#include "GmTouchableFilterPhysicalVolume.hh"
 
class GmExitPhysicalVolumeFilter : public GmVVolumeConditionFilter<GmGeomConditionExit,GmTouchableFilterPhysicalVolume>
{
public:
  GmExitPhysicalVolumeFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionExit, GmTouchableFilterPhysicalVolume>::GmVVolumeConditionFilter(name) {}
};

#endif
