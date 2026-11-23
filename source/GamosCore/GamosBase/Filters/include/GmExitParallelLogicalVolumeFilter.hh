#ifndef GmExitParallelLogicalVolumeFilter_hh
#define GmExitParallelLogicalVolumeFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionExitParallel.hh"
#include "GmTouchableFilterLogicalVolume.hh"
 
class GmExitParallelLogicalVolumeFilter : public GmVVolumeConditionFilter<GmGeomConditionExitParallel,GmTouchableFilterLogicalVolume>
{
public:
  GmExitParallelLogicalVolumeFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionExitParallel, GmTouchableFilterLogicalVolume>::GmVVolumeConditionFilter(name) {}
};

#endif
