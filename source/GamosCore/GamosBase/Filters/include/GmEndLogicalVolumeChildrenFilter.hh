#ifndef GmEndLogicalVolumeChildrenFilter_hh
#define GmEndLogicalVolumeChildrenFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionEnd.hh"
#include "GmTouchableFilterLogicalVolumeChildren.hh"
 
class GmEndLogicalVolumeChildrenFilter : public GmVVolumeConditionFilter<GmGeomConditionEnd,GmTouchableFilterLogicalVolumeChildren>
{
public:
  GmEndLogicalVolumeChildrenFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEnd, GmTouchableFilterLogicalVolumeChildren>::GmVVolumeConditionFilter(name) {}
};

#endif
