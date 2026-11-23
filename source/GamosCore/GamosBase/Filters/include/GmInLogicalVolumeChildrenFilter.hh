#ifndef GmInLogicalVolumeChildrenFilter_hh
#define GmInLogicalVolumeChildrenFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionIn.hh"
#include "GmTouchableFilterLogicalVolumeChildren.hh"
 
class GmInLogicalVolumeChildrenFilter : public GmVVolumeConditionFilter<GmGeomConditionIn,GmTouchableFilterLogicalVolumeChildren>
{
public:
  GmInLogicalVolumeChildrenFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionIn, GmTouchableFilterLogicalVolumeChildren>::GmVVolumeConditionFilter(name) {}
};

#endif
