#ifndef GmEnterPhysicalVolumeChildrenFilter_hh
#define GmEnterPhysicalVolumeChildrenFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionEnter.hh"
#include "GmTouchableFilterPhysicalVolumeChildren.hh"
 
class GmEnterPhysicalVolumeChildrenFilter : public GmVVolumeConditionFilter<GmGeomConditionEnter,GmTouchableFilterPhysicalVolumeChildren>
{
public:
  GmEnterPhysicalVolumeChildrenFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEnter, GmTouchableFilterPhysicalVolumeChildren>::GmVVolumeConditionFilter(name) {}
};

#endif
