#ifndef GmInPhysicalVolumeChildrenFilter_hh
#define GmInPhysicalVolumeChildrenFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionIn.hh"
#include "GmTouchableFilterPhysicalVolumeChildren.hh"
 
class GmInPhysicalVolumeChildrenFilter : public GmVVolumeConditionFilter<GmGeomConditionIn,GmTouchableFilterPhysicalVolumeChildren>
{
public:
  GmInPhysicalVolumeChildrenFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionIn, GmTouchableFilterPhysicalVolumeChildren>::GmVVolumeConditionFilter(name) {}
};

#endif
