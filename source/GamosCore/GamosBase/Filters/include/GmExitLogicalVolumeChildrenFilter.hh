#ifndef GmExitLogicalVolumeChildrenFilter_hh
#define GmExitLogicalVolumeChildrenFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionExit.hh"
#include "GmTouchableFilterLogicalVolumeChildren.hh"
 
class GmExitLogicalVolumeChildrenFilter : public GmVVolumeConditionFilter<GmGeomConditionExit,GmTouchableFilterLogicalVolumeChildren>
{
public:
  GmExitLogicalVolumeChildrenFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionExit, GmTouchableFilterLogicalVolumeChildren>::GmVVolumeConditionFilter(name) {}
};

#endif
