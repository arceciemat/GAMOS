#ifndef GmStartPhysicalVolumeChildrenFilter_hh
#define GmStartPhysicalVolumeChildrenFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionStart.hh"
#include "GmTouchableFilterPhysicalVolumeChildren.hh"
 
class GmStartPhysicalVolumeChildrenFilter : public GmVVolumeConditionFilter<GmGeomConditionStart,GmTouchableFilterPhysicalVolumeChildren>
{
public:
  GmStartPhysicalVolumeChildrenFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionStart, GmTouchableFilterPhysicalVolumeChildren>::GmVVolumeConditionFilter(name) {}
};

#endif
