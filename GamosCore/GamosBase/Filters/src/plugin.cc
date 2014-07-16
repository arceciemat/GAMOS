#include "Reflex/PluginService.h"

#include "GmVVolumeConditionFilter.hh"
#include "GmChargedFilter.hh"
#include "GmNeutralFilter.hh"
#include "GmParticleFilter.hh"
#include "GmKineticEnergyFilter.hh"
#include "GmPostKineticEnergyFilter.hh"
#include "GmVertexKineticEnergyFilter.hh"
#include "GmDeltaEnergyFilter.hh"
#include "GmEnergyChangeFilter.hh"
#include "GmDepositedEnergyFilter.hh"
#include "GmInMassLogicalVolumeFilter.hh"
#include "GmInMassPhysicalVolumeFilter.hh"
#include "GmInMassPhysicalVolumeReplicatedFilter.hh"
#include "GmInMassRegionFilter.hh"
#include "GmPrimaryFilter.hh"
#include "GmSecondaryFilter.hh"
#include "GmGammaFilter.hh"
#include "GmElectronFilter.hh"
#include "GmPositronFilter.hh"
#include "GmNeutronFilter.hh"
#include "GmProtonFilter.hh"
#include "GmElectronOrPositronFilter.hh"
#include "GmEMParticleFilter.hh"
#include "GmStepNumberFilter.hh"
#include "GmProcessFilter.hh"
#include "GmParticleProcessFilter.hh"
#include "GmCreatorProcessFilter.hh"
#include "GmInteractionFilter.hh"
#include "GmProdCutOutsideVoxelFilter.hh"
#include "GmProdCutAllProcOutsideVoxelFilter.hh"
#include "GmMinRangeCutOutsideVoxelFilter.hh"
#include "GmFilterFromClassifier.hh"
#include "GmMaterialFilter.hh"
#include "GmPositionXFilter.hh"
#include "GmPositionYFilter.hh"
#include "GmPositionZFilter.hh"
#include "GmDirectionThetaFilter.hh"
#include "GmDirectionPhiFilter.hh"
#include "GmPositionPreXFilter.hh"
#include "GmPositionPreYFilter.hh"
#include "GmPositionPreZFilter.hh"
#include "GmDirectionPreThetaFilter.hh"
#include "GmDirectionPrePhiFilter.hh"

#include "GmORFilter.hh"
#include "GmXORFilter.hh"
#include "GmANDFilter.hh"
#include "GmOnSecondaryFilter.hh"
#include "GmPrimaryOnAllSecondariesFilter.hh"
#include "GmPrimaryOn1SecondaryFilter.hh"
#include "GmHistoryFilter.hh"
#include "GmPastHistoryFilter.hh"
#include "GmHistoryOrAncestorsFilter.hh"
#include "GmHistoryAllFilter.hh"
#include "GmHistoryAncestorsAllFilter.hh"
#include "GmAncestorsFilter.hh"
#include "GmParentFilter.hh"
#include "GmInverseFilter.hh"

#include "GmGeomConditionEnter.hh"
#include "GmGeomConditionExit.hh"
#include "GmGeomConditionStart.hh"
#include "GmGeomConditionEnd.hh"
#include "GmGeomConditionTraverse.hh"
#include "GmGeomConditionIn.hh"
//#include "GmGeomConditionInMass.hh"
#include "GmGeomConditionEnterParallel.hh"
#include "GmGeomConditionExitParallel.hh"
#include "GmGeomConditionStartParallel.hh"
#include "GmGeomConditionEndParallel.hh"
#include "GmGeomConditionTraverseParallel.hh"
#include "GmGeomConditionInParallel.hh"
#include "GmTouchableFilterLogicalVolume.hh"
#include "GmTouchableFilterPhysicalVolume.hh"
#include "GmTouchableFilterPhysicalVolumeReplicated.hh"
#include "GmTouchableFilterTouchable.hh"
#include "GmTouchableFilterRegion.hh"
#include "GmTouchableFilterLogicalVolumeChildren.hh"
#include "GmTouchableFilterPhysicalVolumeChildren.hh"
#include "GmTouchableFilterPhysicalVolumeReplicatedChildren.hh"
#include "GmTouchableFilterTouchableChildren.hh"
#include "GmTouchableFilterRegionChildren.hh"

#include "GmVolumeFilters.hh"
//typedef GmVVolumeConditionFilter<GmGeomConditionEnter,GmTouchableFilterLogicalVolume> GmEnterLogicalVolumeFilter;
PLUGINSVC_FACTORY(GmEnterLogicalVolumeFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmExitLogicalVolumeFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmStartLogicalVolumeFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmEndLogicalVolumeFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmTraverseLogicalVolumeFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmInLogicalVolumeFilter,GmVFilter*(G4String))
// this class gives problem in AcceptStep, G4Navigator complains 
//PLUGINSVC_FACTORY(GmInMassLogicalVolumeFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmInMassLogicalVolumeFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmEnterParallelLogicalVolumeFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmExitParallelLogicalVolumeFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmStartParallelLogicalVolumeFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmEndParallelLogicalVolumeFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmInParallelLogicalVolumeFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmTraverseParallelLogicalVolumeFilter,GmVFilter*(G4String))

// logical volume and children filters
PLUGINSVC_FACTORY(GmEnterLogicalVolumeChildrenFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmExitLogicalVolumeChildrenFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmStartLogicalVolumeChildrenFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmEndLogicalVolumeChildrenFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmTraverseLogicalVolumeChildrenFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmInLogicalVolumeChildrenFilter,GmVFilter*(G4String))

// physical volume filters
PLUGINSVC_FACTORY(GmEnterPhysicalVolumeFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmExitPhysicalVolumeFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmStartPhysicalVolumeFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmEndPhysicalVolumeFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmTraversePhysicalVolumeFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmInPhysicalVolumeFilter,GmVFilter*(G4String))
// this class gives problem in AcceptStep, G4Navigator complains 
//PLUGINSVC_FACTORY(GmInMassPhysicalVolumeFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmInMassPhysicalVolumeFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmEnterParallelPhysicalVolumeFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmExitParallelPhysicalVolumeFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmStartParallelPhysicalVolumeFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmEndParallelPhysicalVolumeFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmInParallelPhysicalVolumeFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmTraverseParallelPhysicalVolumeFilter,GmVFilter*(G4String))

// physical volume filters
PLUGINSVC_FACTORY(GmEnterPhysicalVolumeReplicatedFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmExitPhysicalVolumeReplicatedFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmStartPhysicalVolumeReplicatedFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmEndPhysicalVolumeReplicatedFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmTraversePhysicalVolumeReplicatedFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmInPhysicalVolumeReplicatedFilter,GmVFilter*(G4String))
// this class gives problem in AcceptStep, G4Navigator complains 
//PLUGINSVC_FACTORY(GmInMassPhysicalVolumeReplicatedFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmInMassPhysicalVolumeReplicatedFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmEnterParallelPhysicalVolumeReplicatedFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmExitParallelPhysicalVolumeReplicatedFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmStartParallelPhysicalVolumeReplicatedFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmEndParallelPhysicalVolumeReplicatedFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmInParallelPhysicalVolumeReplicatedFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmTraverseParallelPhysicalVolumeReplicatedFilter,GmVFilter*(G4String))


// physical volume and children filters
PLUGINSVC_FACTORY(GmEnterPhysicalVolumeChildrenFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmExitPhysicalVolumeChildrenFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmStartPhysicalVolumeChildrenFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmEndPhysicalVolumeChildrenFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmTraversePhysicalVolumeChildrenFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmInPhysicalVolumeChildrenFilter,GmVFilter*(G4String))

// physical volume and children filters
PLUGINSVC_FACTORY(GmEnterPhysicalVolumeChildrenReplicatedFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmExitPhysicalVolumeChildrenReplicatedFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmStartPhysicalVolumeChildrenReplicatedFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmEndPhysicalVolumeChildrenReplicatedFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmTraversePhysicalVolumeChildrenReplicatedFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmInPhysicalVolumeChildrenReplicatedFilter,GmVFilter*(G4String))

// touchable filters
PLUGINSVC_FACTORY(GmEnterTouchableFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmExitTouchableFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmStartTouchableFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmEndTouchableFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmTraverseTouchableFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmInTouchableFilter,GmVFilter*(G4String))

// touchable and children filters
PLUGINSVC_FACTORY(GmEnterTouchableChildrenFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmExitTouchableChildrenFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmStartTouchableChildrenFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmEndTouchableChildrenFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmTraverseTouchableChildrenFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmInTouchableChildrenFilter,GmVFilter*(G4String))

// region filters
PLUGINSVC_FACTORY(GmEnterRegionFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmExitRegionFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmStartRegionFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmEndRegionFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmTraverseRegionFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmInRegionFilter,GmVFilter*(G4String))
// this class gives problem in AcceptStep, G4Navigator complains 
//PLUGINSVC_FACTORY(GmInMassRegionFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmInMassRegionFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmEnterParallelRegionFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmExitParallelRegionFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmStartParallelRegionFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmEndParallelRegionFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmInParallelRegionFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmTraverseParallelRegionFilter,GmVFilter*(G4String))

// region and children filters
PLUGINSVC_FACTORY(GmEnterRegionChildrenFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmExitRegionChildrenFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmStartRegionChildrenFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmEndRegionChildrenFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmTraverseRegionChildrenFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmInRegionChildrenFilter,GmVFilter*(G4String))

#include "GmSameLogicalVolumeFilter.hh"
#include "GmDifferentLogicalVolumeFilter.hh"
PLUGINSVC_FACTORY(GmSameLogicalVolumeFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmDifferentLogicalVolumeFilter,GmVFilter*(G4String))

PLUGINSVC_FACTORY(GmChargedFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmNeutralFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmParticleFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmKineticEnergyFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmPostKineticEnergyFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmVertexKineticEnergyFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmDeltaEnergyFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmEnergyChangeFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmDepositedEnergyFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmPrimaryFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmSecondaryFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmGammaFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmElectronFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmPositronFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmNeutronFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmProtonFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmElectronOrPositronFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmEMParticleFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmStepNumberFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmProcessFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmParticleProcessFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmCreatorProcessFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmInteractionFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmMaterialFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmPositionXFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmPositionYFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmPositionZFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmDirectionThetaFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmDirectionPhiFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmPositionPreXFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmPositionPreYFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmPositionPreZFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmDirectionPreThetaFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmDirectionPrePhiFilter,GmVFilter*(G4String))

PLUGINSVC_FACTORY(GmProdCutOutsideVoxelFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmProdCutAllProcOutsideVoxelFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmMinRangeCutOutsideVoxelFilter,GmVFilter*(G4String))

PLUGINSVC_FACTORY(GmORFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmXORFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmANDFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmOnSecondaryFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmPrimaryOnAllSecondariesFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmPrimaryOn1SecondaryFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmHistoryFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmPastHistoryFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmHistoryOrAncestorsFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmHistoryAllFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmHistoryAncestorsAllFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmAncestorsFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmParentFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmInverseFilter,GmVFilter*(G4String))

PLUGINSVC_FACTORY(GmFilterFromClassifier,GmVFilter*(G4String))

