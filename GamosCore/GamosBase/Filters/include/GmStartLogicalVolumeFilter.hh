#ifndef GmStartLogicalVolumeFilter_hh
#define GmStartLogicalVolumeFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionStart.hh"
#include "GmTouchableFilterLogicalVolume.hh"
 
class GmStartLogicalVolumeFilter : public GmVVolumeConditionFilter<GmGeomConditionStart,GmTouchableFilterLogicalVolume>
{
public:
  GmStartLogicalVolumeFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionStart, GmTouchableFilterLogicalVolume>::GmVVolumeConditionFilter(name) {}
};

#endif
