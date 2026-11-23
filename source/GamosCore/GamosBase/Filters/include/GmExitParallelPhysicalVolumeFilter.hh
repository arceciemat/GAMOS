#ifndef GmExitParallelPhysicalVolumeFilter_hh
#define GmExitParallelPhysicalVolumeFilter_hh
 
#include "GmVVolumeConditionFilter.hh"
#include "GmGeomConditionExitParallel.hh"
#include "GmTouchableFilterPhysicalVolume.hh"
 
class GmExitParallelPhysicalVolumeFilter : public GmVVolumeConditionFilter<GmGeomConditionExitParallel,GmTouchableFilterPhysicalVolume>
{
public:
  GmExitParallelPhysicalVolumeFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionExitParallel, GmTouchableFilterPhysicalVolume>::GmVVolumeConditionFilter(name) {}
};

#endif
