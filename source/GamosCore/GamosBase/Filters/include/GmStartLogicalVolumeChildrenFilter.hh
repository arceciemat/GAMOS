#ifndef GmStartLogicalVolumeChildrenFilter_hh
#define GmStartLogicalVolumeChildrenFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionStart.hh"
#include "GmTouchableFilterLogicalVolumeChildren.hh"
 
class GmStartLogicalVolumeChildrenFilter : public GmVVolumeConditionFilter<GmGeomConditionStart,GmTouchableFilterLogicalVolumeChildren>
{
public:
  GmStartLogicalVolumeChildrenFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionStart, GmTouchableFilterLogicalVolumeChildren>::GmVVolumeConditionFilter(name) {}
};

#endif
