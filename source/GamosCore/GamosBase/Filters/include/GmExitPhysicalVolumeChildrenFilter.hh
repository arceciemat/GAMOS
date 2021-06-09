#ifndef GmExitPhysicalVolumeChildrenFilter_hh
#define GmExitPhysicalVolumeChildrenFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionExit.hh"
#include "GmTouchableFilterPhysicalVolumeChildren.hh"
 
class GmExitPhysicalVolumeChildrenFilter : public GmVVolumeConditionFilter<GmGeomConditionExit,GmTouchableFilterPhysicalVolumeChildren>
{
public:
  GmExitPhysicalVolumeChildrenFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionExit, GmTouchableFilterPhysicalVolumeChildren>::GmVVolumeConditionFilter(name) {}
};

#endif
