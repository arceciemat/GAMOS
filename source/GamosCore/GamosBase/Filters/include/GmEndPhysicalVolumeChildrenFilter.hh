#ifndef GmEndPhysicalVolumeChildrenFilter_hh
#define GmEndPhysicalVolumeChildrenFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionEnd.hh"
#include "GmTouchableFilterPhysicalVolumeChildren.hh"
 
class GmEndPhysicalVolumeChildrenFilter : public GmVVolumeConditionFilter<GmGeomConditionEnd,GmTouchableFilterPhysicalVolumeChildren>
{
public:
  GmEndPhysicalVolumeChildrenFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEnd, GmTouchableFilterPhysicalVolumeChildren>::GmVVolumeConditionFilter(name) {}
};

#endif
