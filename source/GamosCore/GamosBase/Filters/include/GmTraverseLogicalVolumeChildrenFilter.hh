#ifndef GmTraverseLogicalVolumeChildrenFilter_hh
#define GmTraverseLogicalVolumeChildrenFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionTraverse.hh"
#include "GmTouchableFilterLogicalVolumeChildren.hh"
 
class GmTraverseLogicalVolumeChildrenFilter : public GmVVolumeConditionFilter<GmGeomConditionTraverse,GmTouchableFilterLogicalVolumeChildren>
{
public:
  GmTraverseLogicalVolumeChildrenFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionTraverse, GmTouchableFilterLogicalVolumeChildren>::GmVVolumeConditionFilter(name) {}
};

#endif
