#ifndef GmEnterLogicalVolumeChildrenFilter_hh
#define GmEnterLogicalVolumeChildrenFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionEnter.hh"
#include "GmTouchableFilterLogicalVolumeChildren.hh"
 
class GmEnterLogicalVolumeChildrenFilter : public GmVVolumeConditionFilter<GmGeomConditionEnter,GmTouchableFilterLogicalVolumeChildren>
{
public:
  GmEnterLogicalVolumeChildrenFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEnter, GmTouchableFilterLogicalVolumeChildren>::GmVVolumeConditionFilter(name) {}
};

#endif
