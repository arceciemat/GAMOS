#ifndef GmExitLogicalVolumeFilter_hh
#define GmExitLogicalVolumeFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionExit.hh"
#include "GmTouchableFilterLogicalVolume.hh"
 
class GmExitLogicalVolumeFilter : public GmVVolumeConditionFilter<GmGeomConditionExit,GmTouchableFilterLogicalVolume>
{
public:
  GmExitLogicalVolumeFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionExit, GmTouchableFilterLogicalVolume>::GmVVolumeConditionFilter(name) {}
};

#endif
