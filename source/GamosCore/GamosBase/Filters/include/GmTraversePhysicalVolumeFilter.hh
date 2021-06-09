#ifndef GmTraversePhysicalVolumeFilter_hh
#define GmTraversePhysicalVolumeFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionTraverse.hh"
#include "GmTouchableFilterPhysicalVolume.hh"
 
class GmTraversePhysicalVolumeFilter : public GmVVolumeConditionFilter<GmGeomConditionTraverse,GmTouchableFilterPhysicalVolume>
{
public:
  GmTraversePhysicalVolumeFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionTraverse, GmTouchableFilterPhysicalVolume>::GmVVolumeConditionFilter(name) {}
};

#endif
