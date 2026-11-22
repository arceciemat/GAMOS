#ifndef GmTraversePhysicalVolumeChildrenFilter_hh
#define GmTraversePhysicalVolumeChildrenFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionTraverse.hh"
#include "GmTouchableFilterPhysicalVolumeChildren.hh"
 
class GmTraversePhysicalVolumeChildrenFilter : public GmVVolumeConditionFilter<GmGeomConditionTraverse,GmTouchableFilterPhysicalVolumeChildren>
{
public:
  GmTraversePhysicalVolumeChildrenFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionTraverse, GmTouchableFilterPhysicalVolumeChildren>::GmVVolumeConditionFilter(name) {}
};

#endif
