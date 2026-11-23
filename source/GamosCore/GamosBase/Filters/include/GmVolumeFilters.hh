#ifndef GmVolumeFilters_hh
#define GmVolumeFilters_hh
class GmEnterLogicalVolumeFilter : public GmVVolumeConditionFilter<GmGeomConditionEnter,GmTouchableFilterLogicalVolume>
{
public:
  GmEnterLogicalVolumeFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEnter,GmTouchableFilterLogicalVolume>(name) {}
};

class GmExitLogicalVolumeFilter : public GmVVolumeConditionFilter<GmGeomConditionExit,GmTouchableFilterLogicalVolume>
{
public:
  GmExitLogicalVolumeFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionExit,GmTouchableFilterLogicalVolume>(name) {}
};

class GmStartLogicalVolumeFilter : public GmVVolumeConditionFilter<GmGeomConditionStart,GmTouchableFilterLogicalVolume>
{
public:
  GmStartLogicalVolumeFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionStart,GmTouchableFilterLogicalVolume>(name) {}
};

class GmEndLogicalVolumeFilter : public GmVVolumeConditionFilter<GmGeomConditionEnd,GmTouchableFilterLogicalVolume>
{
public:
  GmEndLogicalVolumeFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEnd,GmTouchableFilterLogicalVolume>(name) {}
};

class GmTraverseLogicalVolumeFilter : public GmVVolumeConditionFilter<GmGeomConditionTraverse,GmTouchableFilterLogicalVolume>
{
public:
  GmTraverseLogicalVolumeFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionTraverse,GmTouchableFilterLogicalVolume>(name) {}
};

class GmInLogicalVolumeFilter : public GmVVolumeConditionFilter<GmGeomConditionIn,GmTouchableFilterLogicalVolume>
{
public:
  GmInLogicalVolumeFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionIn,GmTouchableFilterLogicalVolume>(name) {}
};

class GmEnterParallelLogicalVolumeFilter : public GmVVolumeConditionFilter<GmGeomConditionEnterParallel,GmTouchableFilterLogicalVolume>
{
public:
  GmEnterParallelLogicalVolumeFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEnterParallel,GmTouchableFilterLogicalVolume>(name) {}
};

class GmExitParallelLogicalVolumeFilter : public GmVVolumeConditionFilter<GmGeomConditionExitParallel,GmTouchableFilterLogicalVolume>
{
public:
  GmExitParallelLogicalVolumeFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionExitParallel,GmTouchableFilterLogicalVolume>(name) {}
};

class GmStartParallelLogicalVolumeFilter : public GmVVolumeConditionFilter<GmGeomConditionStartParallel,GmTouchableFilterLogicalVolume>
{
public:
  GmStartParallelLogicalVolumeFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionStartParallel,GmTouchableFilterLogicalVolume>(name) {}
};

class GmEndParallelLogicalVolumeFilter : public GmVVolumeConditionFilter<GmGeomConditionEndParallel,GmTouchableFilterLogicalVolume>
{
public:
  GmEndParallelLogicalVolumeFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEndParallel,GmTouchableFilterLogicalVolume>(name) {}
};

class GmInParallelLogicalVolumeFilter : public GmVVolumeConditionFilter<GmGeomConditionInParallel,GmTouchableFilterLogicalVolume>
{
public:
  GmInParallelLogicalVolumeFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionInParallel,GmTouchableFilterLogicalVolume>(name) {}
};

class GmTraverseParallelLogicalVolumeFilter : public GmVVolumeConditionFilter<GmGeomConditionTraverseParallel,GmTouchableFilterLogicalVolume>
{
public:
  GmTraverseParallelLogicalVolumeFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionTraverseParallel,GmTouchableFilterLogicalVolume>(name) {}
};

class GmEnterLogicalVolumeChildrenFilter : public GmVVolumeConditionFilter<GmGeomConditionEnter,GmTouchableFilterLogicalVolumeChildren>
{
public:
  GmEnterLogicalVolumeChildrenFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEnter,GmTouchableFilterLogicalVolumeChildren>(name) {}
};

class GmExitLogicalVolumeChildrenFilter : public GmVVolumeConditionFilter<GmGeomConditionExit,GmTouchableFilterLogicalVolumeChildren>
{
public:
  GmExitLogicalVolumeChildrenFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionExit,GmTouchableFilterLogicalVolumeChildren>(name) {}
};

class GmStartLogicalVolumeChildrenFilter : public GmVVolumeConditionFilter<GmGeomConditionStart,GmTouchableFilterLogicalVolumeChildren>
{
public:
  GmStartLogicalVolumeChildrenFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionStart,GmTouchableFilterLogicalVolumeChildren>(name) {}
};

class GmEndLogicalVolumeChildrenFilter : public GmVVolumeConditionFilter<GmGeomConditionEnd,GmTouchableFilterLogicalVolumeChildren>
{
public:
  GmEndLogicalVolumeChildrenFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEnd,GmTouchableFilterLogicalVolumeChildren>(name) {}
};

class GmTraverseLogicalVolumeChildrenFilter : public GmVVolumeConditionFilter<GmGeomConditionTraverse,GmTouchableFilterLogicalVolumeChildren>
{
public:
  GmTraverseLogicalVolumeChildrenFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionTraverse,GmTouchableFilterLogicalVolumeChildren>(name) {}
};

class GmInLogicalVolumeChildrenFilter : public GmVVolumeConditionFilter<GmGeomConditionIn,GmTouchableFilterLogicalVolumeChildren>
{
public:
  GmInLogicalVolumeChildrenFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionIn,GmTouchableFilterLogicalVolumeChildren>(name) {}
};

class GmEnterPhysicalVolumeFilter : public GmVVolumeConditionFilter<GmGeomConditionEnter,GmTouchableFilterPhysicalVolume>
{
public:
  GmEnterPhysicalVolumeFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEnter,GmTouchableFilterPhysicalVolume>(name) {}
};

class GmExitPhysicalVolumeFilter : public GmVVolumeConditionFilter<GmGeomConditionExit,GmTouchableFilterPhysicalVolume>
{
public:
  GmExitPhysicalVolumeFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionExit,GmTouchableFilterPhysicalVolume>(name) {}
};

class GmStartPhysicalVolumeFilter : public GmVVolumeConditionFilter<GmGeomConditionStart,GmTouchableFilterPhysicalVolume>
{
public:
  GmStartPhysicalVolumeFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionStart,GmTouchableFilterPhysicalVolume>(name) {}
};

class GmEndPhysicalVolumeFilter : public GmVVolumeConditionFilter<GmGeomConditionEnd,GmTouchableFilterPhysicalVolume>
{
public:
  GmEndPhysicalVolumeFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEnd,GmTouchableFilterPhysicalVolume>(name) {}
};

class GmTraversePhysicalVolumeFilter : public GmVVolumeConditionFilter<GmGeomConditionTraverse,GmTouchableFilterPhysicalVolume>
{
public:
  GmTraversePhysicalVolumeFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionTraverse,GmTouchableFilterPhysicalVolume>(name) {}
};

class GmInPhysicalVolumeFilter : public GmVVolumeConditionFilter<GmGeomConditionIn,GmTouchableFilterPhysicalVolume>
{
public:
  GmInPhysicalVolumeFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionIn,GmTouchableFilterPhysicalVolume>(name) {}
};

class GmEnterParallelPhysicalVolumeFilter : public GmVVolumeConditionFilter<GmGeomConditionEnterParallel,GmTouchableFilterPhysicalVolume>
{
public:
  GmEnterParallelPhysicalVolumeFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEnterParallel,GmTouchableFilterPhysicalVolume>(name) {}
};

class GmExitParallelPhysicalVolumeFilter : public GmVVolumeConditionFilter<GmGeomConditionExitParallel,GmTouchableFilterPhysicalVolume>
{
public:
  GmExitParallelPhysicalVolumeFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionExitParallel,GmTouchableFilterPhysicalVolume>(name) {}
};

class GmStartParallelPhysicalVolumeFilter : public GmVVolumeConditionFilter<GmGeomConditionStartParallel,GmTouchableFilterPhysicalVolume>
{
public:
  GmStartParallelPhysicalVolumeFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionStartParallel,GmTouchableFilterPhysicalVolume>(name) {}
};

class GmEndParallelPhysicalVolumeFilter : public GmVVolumeConditionFilter<GmGeomConditionEndParallel,GmTouchableFilterPhysicalVolume>
{
public:
  GmEndParallelPhysicalVolumeFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEndParallel,GmTouchableFilterPhysicalVolume>(name) {}
};

class GmInParallelPhysicalVolumeFilter : public GmVVolumeConditionFilter<GmGeomConditionInParallel,GmTouchableFilterPhysicalVolume>
{
public:
  GmInParallelPhysicalVolumeFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionInParallel,GmTouchableFilterPhysicalVolume>(name) {}
};

class GmTraverseParallelPhysicalVolumeFilter : public GmVVolumeConditionFilter<GmGeomConditionTraverseParallel,GmTouchableFilterPhysicalVolume>
{
public:
  GmTraverseParallelPhysicalVolumeFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionTraverseParallel,GmTouchableFilterPhysicalVolume>(name) {}
};

class GmEnterPhysicalVolumeReplicatedFilter : public GmVVolumeConditionFilter<GmGeomConditionEnter,GmTouchableFilterPhysicalVolumeReplicated>
{
public:
  GmEnterPhysicalVolumeReplicatedFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEnter,GmTouchableFilterPhysicalVolumeReplicated>(name) {}
};

class GmExitPhysicalVolumeReplicatedFilter : public GmVVolumeConditionFilter<GmGeomConditionExit,GmTouchableFilterPhysicalVolumeReplicated>
{
public:
  GmExitPhysicalVolumeReplicatedFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionExit,GmTouchableFilterPhysicalVolumeReplicated>(name) {}
};

class GmStartPhysicalVolumeReplicatedFilter : public GmVVolumeConditionFilter<GmGeomConditionStart,GmTouchableFilterPhysicalVolumeReplicated>
{
public:
  GmStartPhysicalVolumeReplicatedFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionStart,GmTouchableFilterPhysicalVolumeReplicated>(name) {}
};

class GmEndPhysicalVolumeReplicatedFilter : public GmVVolumeConditionFilter<GmGeomConditionEnd,GmTouchableFilterPhysicalVolumeReplicated>
{
public:
  GmEndPhysicalVolumeReplicatedFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEnd,GmTouchableFilterPhysicalVolumeReplicated>(name) {}
};

class GmTraversePhysicalVolumeReplicatedFilter : public GmVVolumeConditionFilter<GmGeomConditionTraverse,GmTouchableFilterPhysicalVolumeReplicated>
{
public:
  GmTraversePhysicalVolumeReplicatedFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionTraverse,GmTouchableFilterPhysicalVolumeReplicated>(name) {}
};

class GmInPhysicalVolumeReplicatedFilter : public GmVVolumeConditionFilter<GmGeomConditionIn,GmTouchableFilterPhysicalVolumeReplicated>
{
public:
  GmInPhysicalVolumeReplicatedFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionIn,GmTouchableFilterPhysicalVolumeReplicated>(name) {}
};

class GmEnterParallelPhysicalVolumeReplicatedFilter : public GmVVolumeConditionFilter<GmGeomConditionEnterParallel,GmTouchableFilterPhysicalVolumeReplicated>
{
public:
  GmEnterParallelPhysicalVolumeReplicatedFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEnterParallel,GmTouchableFilterPhysicalVolumeReplicated>(name) {}
};

class GmExitParallelPhysicalVolumeReplicatedFilter : public GmVVolumeConditionFilter<GmGeomConditionExitParallel,GmTouchableFilterPhysicalVolumeReplicated>
{
public:
  GmExitParallelPhysicalVolumeReplicatedFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionExitParallel,GmTouchableFilterPhysicalVolumeReplicated>(name) {}
};

class GmStartParallelPhysicalVolumeReplicatedFilter : public GmVVolumeConditionFilter<GmGeomConditionStartParallel,GmTouchableFilterPhysicalVolumeReplicated>
{
public:
  GmStartParallelPhysicalVolumeReplicatedFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionStartParallel,GmTouchableFilterPhysicalVolumeReplicated>(name) {}
};

class GmEndParallelPhysicalVolumeReplicatedFilter : public GmVVolumeConditionFilter<GmGeomConditionEndParallel,GmTouchableFilterPhysicalVolumeReplicated>
{
public:
  GmEndParallelPhysicalVolumeReplicatedFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEndParallel,GmTouchableFilterPhysicalVolumeReplicated>(name) {}
};

class GmInParallelPhysicalVolumeReplicatedFilter : public GmVVolumeConditionFilter<GmGeomConditionInParallel,GmTouchableFilterPhysicalVolumeReplicated>
{
public:
  GmInParallelPhysicalVolumeReplicatedFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionInParallel,GmTouchableFilterPhysicalVolumeReplicated>(name) {}
};

class GmTraverseParallelPhysicalVolumeReplicatedFilter : public GmVVolumeConditionFilter<GmGeomConditionTraverseParallel,GmTouchableFilterPhysicalVolumeReplicated>
{
public:
  GmTraverseParallelPhysicalVolumeReplicatedFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionTraverseParallel,GmTouchableFilterPhysicalVolumeReplicated>(name) {}
};

class GmEnterPhysicalVolumeChildrenFilter : public GmVVolumeConditionFilter<GmGeomConditionEnter,GmTouchableFilterPhysicalVolumeChildren>
{
public:
  GmEnterPhysicalVolumeChildrenFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEnter,GmTouchableFilterPhysicalVolumeChildren>(name) {}
};

class GmExitPhysicalVolumeChildrenFilter : public GmVVolumeConditionFilter<GmGeomConditionExit,GmTouchableFilterPhysicalVolumeChildren>
{
public:
  GmExitPhysicalVolumeChildrenFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionExit,GmTouchableFilterPhysicalVolumeChildren>(name) {}
};

class GmStartPhysicalVolumeChildrenFilter : public GmVVolumeConditionFilter<GmGeomConditionStart,GmTouchableFilterPhysicalVolumeChildren>
{
public:
  GmStartPhysicalVolumeChildrenFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionStart,GmTouchableFilterPhysicalVolumeChildren>(name) {}
};

class GmEndPhysicalVolumeChildrenFilter : public GmVVolumeConditionFilter<GmGeomConditionEnd,GmTouchableFilterPhysicalVolumeChildren>
{
public:
  GmEndPhysicalVolumeChildrenFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEnd,GmTouchableFilterPhysicalVolumeChildren>(name) {}
};

class GmTraversePhysicalVolumeChildrenFilter : public GmVVolumeConditionFilter<GmGeomConditionTraverse,GmTouchableFilterPhysicalVolumeChildren>
{
public:
  GmTraversePhysicalVolumeChildrenFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionTraverse,GmTouchableFilterPhysicalVolumeChildren>(name) {}
};

class GmInPhysicalVolumeChildrenFilter : public GmVVolumeConditionFilter<GmGeomConditionIn,GmTouchableFilterPhysicalVolumeChildren>
{
public:
  GmInPhysicalVolumeChildrenFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionIn,GmTouchableFilterPhysicalVolumeChildren>(name) {}
};

class GmEnterPhysicalVolumeChildrenReplicatedFilter : public GmVVolumeConditionFilter<GmGeomConditionEnter,GmTouchableFilterPhysicalVolumeReplicatedChildren>
{
public:
  GmEnterPhysicalVolumeChildrenReplicatedFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEnter,GmTouchableFilterPhysicalVolumeReplicatedChildren>(name) {}
};

class GmExitPhysicalVolumeChildrenReplicatedFilter : public GmVVolumeConditionFilter<GmGeomConditionExit,GmTouchableFilterPhysicalVolumeReplicatedChildren>
{
public:
  GmExitPhysicalVolumeChildrenReplicatedFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionExit,GmTouchableFilterPhysicalVolumeReplicatedChildren>(name) {}
};

class GmStartPhysicalVolumeChildrenReplicatedFilter : public GmVVolumeConditionFilter<GmGeomConditionStart,GmTouchableFilterPhysicalVolumeReplicatedChildren>
{
public:
  GmStartPhysicalVolumeChildrenReplicatedFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionStart,GmTouchableFilterPhysicalVolumeReplicatedChildren>(name) {}
};

class GmEndPhysicalVolumeChildrenReplicatedFilter : public GmVVolumeConditionFilter<GmGeomConditionEnd,GmTouchableFilterPhysicalVolumeReplicatedChildren>
{
public:
  GmEndPhysicalVolumeChildrenReplicatedFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEnd,GmTouchableFilterPhysicalVolumeReplicatedChildren>(name) {}
};

class GmTraversePhysicalVolumeChildrenReplicatedFilter : public GmVVolumeConditionFilter<GmGeomConditionTraverse,GmTouchableFilterPhysicalVolumeReplicatedChildren>
{
public:
  GmTraversePhysicalVolumeChildrenReplicatedFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionTraverse,GmTouchableFilterPhysicalVolumeReplicatedChildren>(name) {}
};

class GmInPhysicalVolumeChildrenReplicatedFilter : public GmVVolumeConditionFilter<GmGeomConditionIn,GmTouchableFilterPhysicalVolumeReplicatedChildren>
{
public:
  GmInPhysicalVolumeChildrenReplicatedFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionIn,GmTouchableFilterPhysicalVolumeReplicatedChildren>(name) {}
};

class GmEnterTouchableFilter : public GmVVolumeConditionFilter<GmGeomConditionEnter,GmTouchableFilterTouchable>
{
public:
  GmEnterTouchableFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEnter,GmTouchableFilterTouchable>(name) {}
};

class GmExitTouchableFilter : public GmVVolumeConditionFilter<GmGeomConditionExit,GmTouchableFilterTouchable>
{
public:
  GmExitTouchableFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionExit,GmTouchableFilterTouchable>(name) {}
};

class GmStartTouchableFilter : public GmVVolumeConditionFilter<GmGeomConditionStart,GmTouchableFilterTouchable>
{
public:
  GmStartTouchableFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionStart,GmTouchableFilterTouchable>(name) {}
};

class GmEndTouchableFilter : public GmVVolumeConditionFilter<GmGeomConditionEnd,GmTouchableFilterTouchable>
{
public:
  GmEndTouchableFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEnd,GmTouchableFilterTouchable>(name) {}
};

class GmTraverseTouchableFilter : public GmVVolumeConditionFilter<GmGeomConditionTraverse,GmTouchableFilterTouchable>
{
public:
  GmTraverseTouchableFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionTraverse,GmTouchableFilterTouchable>(name) {}
};

class GmInTouchableFilter : public GmVVolumeConditionFilter<GmGeomConditionIn,GmTouchableFilterTouchable>
{
public:
  GmInTouchableFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionIn,GmTouchableFilterTouchable>(name) {}
};

class GmEnterTouchableChildrenFilter : public GmVVolumeConditionFilter<GmGeomConditionEnter,GmTouchableFilterTouchableChildren>
{
public:
  GmEnterTouchableChildrenFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEnter,GmTouchableFilterTouchableChildren>(name) {}
};

class GmExitTouchableChildrenFilter : public GmVVolumeConditionFilter<GmGeomConditionExit,GmTouchableFilterTouchableChildren>
{
public:
  GmExitTouchableChildrenFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionExit,GmTouchableFilterTouchableChildren>(name) {}
};

class GmStartTouchableChildrenFilter : public GmVVolumeConditionFilter<GmGeomConditionStart,GmTouchableFilterTouchableChildren>
{
public:
  GmStartTouchableChildrenFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionStart,GmTouchableFilterTouchableChildren>(name) {}
};

class GmEndTouchableChildrenFilter : public GmVVolumeConditionFilter<GmGeomConditionEnd,GmTouchableFilterTouchableChildren>
{
public:
  GmEndTouchableChildrenFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEnd,GmTouchableFilterTouchableChildren>(name) {}
};

class GmTraverseTouchableChildrenFilter : public GmVVolumeConditionFilter<GmGeomConditionTraverse,GmTouchableFilterTouchableChildren>
{
public:
  GmTraverseTouchableChildrenFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionTraverse,GmTouchableFilterTouchableChildren>(name) {}
};

class GmInTouchableChildrenFilter : public GmVVolumeConditionFilter<GmGeomConditionIn,GmTouchableFilterTouchableChildren>
{
public:
  GmInTouchableChildrenFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionIn,GmTouchableFilterTouchableChildren>(name) {}
};

class GmEnterRegionFilter : public GmVVolumeConditionFilter<GmGeomConditionEnter,GmTouchableFilterRegion>
{
public:
  GmEnterRegionFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEnter,GmTouchableFilterRegion>(name) {}
};

class GmExitRegionFilter : public GmVVolumeConditionFilter<GmGeomConditionExit,GmTouchableFilterRegion>
{
public:
  GmExitRegionFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionExit,GmTouchableFilterRegion>(name) {}
};

class GmStartRegionFilter : public GmVVolumeConditionFilter<GmGeomConditionStart,GmTouchableFilterRegion>
{
public:
  GmStartRegionFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionStart,GmTouchableFilterRegion>(name) {}
};

class GmEndRegionFilter : public GmVVolumeConditionFilter<GmGeomConditionEnd,GmTouchableFilterRegion>
{
public:
  GmEndRegionFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEnd,GmTouchableFilterRegion>(name) {}
};

class GmTraverseRegionFilter : public GmVVolumeConditionFilter<GmGeomConditionTraverse,GmTouchableFilterRegion>
{
public:
  GmTraverseRegionFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionTraverse,GmTouchableFilterRegion>(name) {}
};

class GmInRegionFilter : public GmVVolumeConditionFilter<GmGeomConditionIn,GmTouchableFilterRegion>
{
public:
  GmInRegionFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionIn,GmTouchableFilterRegion>(name) {}
};

class GmEnterParallelRegionFilter : public GmVVolumeConditionFilter<GmGeomConditionEnterParallel,GmTouchableFilterRegion>
{
public:
  GmEnterParallelRegionFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEnterParallel,GmTouchableFilterRegion>(name) {}
};

class GmExitParallelRegionFilter : public GmVVolumeConditionFilter<GmGeomConditionExitParallel,GmTouchableFilterRegion>
{
public:
  GmExitParallelRegionFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionExitParallel,GmTouchableFilterRegion>(name) {}
};

class GmStartParallelRegionFilter : public GmVVolumeConditionFilter<GmGeomConditionStartParallel,GmTouchableFilterRegion>
{
public:
  GmStartParallelRegionFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionStartParallel,GmTouchableFilterRegion>(name) {}
};

class GmEndParallelRegionFilter : public GmVVolumeConditionFilter<GmGeomConditionEndParallel,GmTouchableFilterRegion>
{
public:
  GmEndParallelRegionFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEndParallel,GmTouchableFilterRegion>(name) {}
};

class GmInParallelRegionFilter : public GmVVolumeConditionFilter<GmGeomConditionInParallel,GmTouchableFilterRegion>
{
public:
  GmInParallelRegionFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionInParallel,GmTouchableFilterRegion>(name) {}
};

class GmTraverseParallelRegionFilter : public GmVVolumeConditionFilter<GmGeomConditionTraverseParallel,GmTouchableFilterRegion>
{
public:
  GmTraverseParallelRegionFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionTraverseParallel,GmTouchableFilterRegion>(name) {}
};

class GmEnterRegionChildrenFilter : public GmVVolumeConditionFilter<GmGeomConditionEnter,GmTouchableFilterRegionChildren>
{
public:
  GmEnterRegionChildrenFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEnter,GmTouchableFilterRegionChildren>(name) {}
};

class GmExitRegionChildrenFilter : public GmVVolumeConditionFilter<GmGeomConditionExit,GmTouchableFilterRegionChildren>
{
public:
  GmExitRegionChildrenFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionExit,GmTouchableFilterRegionChildren>(name) {}
};

class GmStartRegionChildrenFilter : public GmVVolumeConditionFilter<GmGeomConditionStart,GmTouchableFilterRegionChildren>
{
public:
  GmStartRegionChildrenFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionStart,GmTouchableFilterRegionChildren>(name) {}
};

class GmEndRegionChildrenFilter : public GmVVolumeConditionFilter<GmGeomConditionEnd,GmTouchableFilterRegionChildren>
{
public:
  GmEndRegionChildrenFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionEnd,GmTouchableFilterRegionChildren>(name) {}
};

class GmTraverseRegionChildrenFilter : public GmVVolumeConditionFilter<GmGeomConditionTraverse,GmTouchableFilterRegionChildren>
{
public:
  GmTraverseRegionChildrenFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionTraverse,GmTouchableFilterRegionChildren>(name) {}
};

class GmInRegionChildrenFilter : public GmVVolumeConditionFilter<GmGeomConditionIn,GmTouchableFilterRegionChildren>
{
public:
  GmInRegionChildrenFilter(G4String name) : GmVVolumeConditionFilter<GmGeomConditionIn,GmTouchableFilterRegionChildren>(name) {}
};

#endif
