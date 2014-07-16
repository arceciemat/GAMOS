#ifndef GmTraverseLogicalVolumeFilter_hh
#define GmTraverseLogicalVolumeFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionTraverse.hh"
#include "GmTouchableFilterLogicalVolume.hh"
 
class GmTraverseLogicalVolumeFilter : public GmVVolumeConditionFilter<GmGeomConditionTraverse,GmTouchableFilterLogicalVolume>
{
public:
  GmTraverseLogicalVolumeFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionTraverse, GmTouchableFilterLogicalVolume>::GmVVolumeConditionFilter(name) {}
};

#endif
