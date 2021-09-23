#include "GmVVolumeConditionFilter.hh"
#include "GmVParallelVolumeConditionFilter.hh"
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
#include "GmHistoryOrAncestorsFilterForScorer.hh"
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

#ifdef ROOT5

#include "Reflex/PluginService.h"

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
PLUGINSVC_FACTORY(GmHistoryOrAncestorsFilterForScorer,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmHistoryAllFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmHistoryAncestorsAllFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmAncestorsFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmParentFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmInverseFilter,GmVFilter*(G4String))

PLUGINSVC_FACTORY(GmFilterFromClassifier,GmVFilter*(G4String))

#include "GmFilterVerbosity.hh"
PLUGINSVC_FACTORY(GmFilterVerbosity, GmVVerbosity*())

#else

#include "SEAL_Foundation/PluginManager/PluginManager/ModuleDef.h"

DEFINE_SEAL_MODULE();

#include "GamosCore/GamosBase/Base/include/GmFilterFactory.hh"

DEFINE_GAMOS_FILTER(GmEnterLogicalVolumeFilter);
DEFINE_GAMOS_FILTER(GmExitLogicalVolumeFilter);
DEFINE_GAMOS_FILTER(GmStartLogicalVolumeFilter);
DEFINE_GAMOS_FILTER(GmEndLogicalVolumeFilter);
DEFINE_GAMOS_FILTER(GmTraverseLogicalVolumeFilter);
DEFINE_GAMOS_FILTER(GmInLogicalVolumeFilter);
// this class gives problem in AcceptStep, G4Navigator complains 
//DEFINE_GAMOS_FILTER(GmInMassLogicalVolumeFilter);
DEFINE_GAMOS_FILTER(GmInMassLogicalVolumeFilter);
DEFINE_GAMOS_FILTER(GmEnterParallelLogicalVolumeFilter);
DEFINE_GAMOS_FILTER(GmExitParallelLogicalVolumeFilter);
DEFINE_GAMOS_FILTER(GmStartParallelLogicalVolumeFilter);
DEFINE_GAMOS_FILTER(GmEndParallelLogicalVolumeFilter);
DEFINE_GAMOS_FILTER(GmInParallelLogicalVolumeFilter);
DEFINE_GAMOS_FILTER(GmTraverseParallelLogicalVolumeFilter);

// logical volume and children filters
DEFINE_GAMOS_FILTER(GmEnterLogicalVolumeChildrenFilter);
DEFINE_GAMOS_FILTER(GmExitLogicalVolumeChildrenFilter);
DEFINE_GAMOS_FILTER(GmStartLogicalVolumeChildrenFilter);
DEFINE_GAMOS_FILTER(GmEndLogicalVolumeChildrenFilter);
DEFINE_GAMOS_FILTER(GmTraverseLogicalVolumeChildrenFilter);
DEFINE_GAMOS_FILTER(GmInLogicalVolumeChildrenFilter);

// physical volume filters
DEFINE_GAMOS_FILTER(GmEnterPhysicalVolumeFilter);
DEFINE_GAMOS_FILTER(GmExitPhysicalVolumeFilter);
DEFINE_GAMOS_FILTER(GmStartPhysicalVolumeFilter);
DEFINE_GAMOS_FILTER(GmEndPhysicalVolumeFilter);
DEFINE_GAMOS_FILTER(GmTraversePhysicalVolumeFilter);
DEFINE_GAMOS_FILTER(GmInPhysicalVolumeFilter);
// this class gives problem in AcceptStep, G4Navigator complains 
//DEFINE_GAMOS_FILTER(GmInMassPhysicalVolumeFilter);
DEFINE_GAMOS_FILTER(GmInMassPhysicalVolumeFilter);
DEFINE_GAMOS_FILTER(GmEnterParallelPhysicalVolumeFilter);
DEFINE_GAMOS_FILTER(GmExitParallelPhysicalVolumeFilter);
DEFINE_GAMOS_FILTER(GmStartParallelPhysicalVolumeFilter);
DEFINE_GAMOS_FILTER(GmEndParallelPhysicalVolumeFilter);
DEFINE_GAMOS_FILTER(GmInParallelPhysicalVolumeFilter);
DEFINE_GAMOS_FILTER(GmTraverseParallelPhysicalVolumeFilter);

// physical volume filters
DEFINE_GAMOS_FILTER(GmEnterPhysicalVolumeReplicatedFilter);
DEFINE_GAMOS_FILTER(GmExitPhysicalVolumeReplicatedFilter);
DEFINE_GAMOS_FILTER(GmStartPhysicalVolumeReplicatedFilter);
DEFINE_GAMOS_FILTER(GmEndPhysicalVolumeReplicatedFilter);
DEFINE_GAMOS_FILTER(GmTraversePhysicalVolumeReplicatedFilter);
DEFINE_GAMOS_FILTER(GmInPhysicalVolumeReplicatedFilter);
// this class gives problem in AcceptStep, G4Navigator complains 
//DEFINE_GAMOS_FILTER(GmInMassPhysicalVolumeReplicatedFilter);
DEFINE_GAMOS_FILTER(GmInMassPhysicalVolumeReplicatedFilter);
DEFINE_GAMOS_FILTER(GmEnterParallelPhysicalVolumeReplicatedFilter);
DEFINE_GAMOS_FILTER(GmExitParallelPhysicalVolumeReplicatedFilter);
DEFINE_GAMOS_FILTER(GmStartParallelPhysicalVolumeReplicatedFilter);
DEFINE_GAMOS_FILTER(GmEndParallelPhysicalVolumeReplicatedFilter);
DEFINE_GAMOS_FILTER(GmInParallelPhysicalVolumeReplicatedFilter);
DEFINE_GAMOS_FILTER(GmTraverseParallelPhysicalVolumeReplicatedFilter);


// physical volume and children filters
DEFINE_GAMOS_FILTER(GmEnterPhysicalVolumeChildrenFilter);
DEFINE_GAMOS_FILTER(GmExitPhysicalVolumeChildrenFilter);
DEFINE_GAMOS_FILTER(GmStartPhysicalVolumeChildrenFilter);
DEFINE_GAMOS_FILTER(GmEndPhysicalVolumeChildrenFilter);
DEFINE_GAMOS_FILTER(GmTraversePhysicalVolumeChildrenFilter);
DEFINE_GAMOS_FILTER(GmInPhysicalVolumeChildrenFilter);

// physical volume and children filters
DEFINE_GAMOS_FILTER(GmEnterPhysicalVolumeChildrenReplicatedFilter);
DEFINE_GAMOS_FILTER(GmExitPhysicalVolumeChildrenReplicatedFilter);
DEFINE_GAMOS_FILTER(GmStartPhysicalVolumeChildrenReplicatedFilter);
DEFINE_GAMOS_FILTER(GmEndPhysicalVolumeChildrenReplicatedFilter);
DEFINE_GAMOS_FILTER(GmTraversePhysicalVolumeChildrenReplicatedFilter);
DEFINE_GAMOS_FILTER(GmInPhysicalVolumeChildrenReplicatedFilter);

// touchable filters
DEFINE_GAMOS_FILTER(GmEnterTouchableFilter);
DEFINE_GAMOS_FILTER(GmExitTouchableFilter);
DEFINE_GAMOS_FILTER(GmStartTouchableFilter);
DEFINE_GAMOS_FILTER(GmEndTouchableFilter);
DEFINE_GAMOS_FILTER(GmTraverseTouchableFilter);
DEFINE_GAMOS_FILTER(GmInTouchableFilter);

// touchable and children filters
DEFINE_GAMOS_FILTER(GmEnterTouchableChildrenFilter);
DEFINE_GAMOS_FILTER(GmExitTouchableChildrenFilter);
DEFINE_GAMOS_FILTER(GmStartTouchableChildrenFilter);
DEFINE_GAMOS_FILTER(GmEndTouchableChildrenFilter);
DEFINE_GAMOS_FILTER(GmTraverseTouchableChildrenFilter);
DEFINE_GAMOS_FILTER(GmInTouchableChildrenFilter);

// region filters
DEFINE_GAMOS_FILTER(GmEnterRegionFilter);
DEFINE_GAMOS_FILTER(GmExitRegionFilter);
DEFINE_GAMOS_FILTER(GmStartRegionFilter);
DEFINE_GAMOS_FILTER(GmEndRegionFilter);
DEFINE_GAMOS_FILTER(GmTraverseRegionFilter);
DEFINE_GAMOS_FILTER(GmInRegionFilter);
// this class gives problem in AcceptStep, G4Navigator complains 
//DEFINE_GAMOS_FILTER(GmInMassRegionFilter);
DEFINE_GAMOS_FILTER(GmInMassRegionFilter);
DEFINE_GAMOS_FILTER(GmEnterParallelRegionFilter);
DEFINE_GAMOS_FILTER(GmExitParallelRegionFilter);
DEFINE_GAMOS_FILTER(GmStartParallelRegionFilter);
DEFINE_GAMOS_FILTER(GmEndParallelRegionFilter);
DEFINE_GAMOS_FILTER(GmInParallelRegionFilter);
DEFINE_GAMOS_FILTER(GmTraverseParallelRegionFilter);

// region and children filters
DEFINE_GAMOS_FILTER(GmEnterRegionChildrenFilter);
DEFINE_GAMOS_FILTER(GmExitRegionChildrenFilter);
DEFINE_GAMOS_FILTER(GmStartRegionChildrenFilter);
DEFINE_GAMOS_FILTER(GmEndRegionChildrenFilter);
DEFINE_GAMOS_FILTER(GmTraverseRegionChildrenFilter);
DEFINE_GAMOS_FILTER(GmInRegionChildrenFilter);

#include "GmSameLogicalVolumeFilter.hh"
#include "GmDifferentLogicalVolumeFilter.hh"
DEFINE_GAMOS_FILTER(GmSameLogicalVolumeFilter);
DEFINE_GAMOS_FILTER(GmDifferentLogicalVolumeFilter);

DEFINE_GAMOS_FILTER(GmChargedFilter);
DEFINE_GAMOS_FILTER(GmNeutralFilter);
DEFINE_GAMOS_FILTER(GmParticleFilter);
DEFINE_GAMOS_FILTER(GmKineticEnergyFilter);
DEFINE_GAMOS_FILTER(GmPostKineticEnergyFilter);
DEFINE_GAMOS_FILTER(GmVertexKineticEnergyFilter);
DEFINE_GAMOS_FILTER(GmDeltaEnergyFilter);
DEFINE_GAMOS_FILTER(GmEnergyChangeFilter);
DEFINE_GAMOS_FILTER(GmDepositedEnergyFilter);
DEFINE_GAMOS_FILTER(GmPrimaryFilter);
DEFINE_GAMOS_FILTER(GmSecondaryFilter);
DEFINE_GAMOS_FILTER(GmGammaFilter);
DEFINE_GAMOS_FILTER(GmElectronFilter);
DEFINE_GAMOS_FILTER(GmPositronFilter);
DEFINE_GAMOS_FILTER(GmNeutronFilter);
DEFINE_GAMOS_FILTER(GmProtonFilter);
DEFINE_GAMOS_FILTER(GmElectronOrPositronFilter);
DEFINE_GAMOS_FILTER(GmEMParticleFilter);
DEFINE_GAMOS_FILTER(GmStepNumberFilter);
DEFINE_GAMOS_FILTER(GmProcessFilter);
DEFINE_GAMOS_FILTER(GmParticleProcessFilter);
DEFINE_GAMOS_FILTER(GmCreatorProcessFilter);
DEFINE_GAMOS_FILTER(GmInteractionFilter);
DEFINE_GAMOS_FILTER(GmMaterialFilter);
DEFINE_GAMOS_FILTER(GmPositionXFilter);
DEFINE_GAMOS_FILTER(GmPositionYFilter);
DEFINE_GAMOS_FILTER(GmPositionZFilter);
DEFINE_GAMOS_FILTER(GmDirectionThetaFilter);
DEFINE_GAMOS_FILTER(GmDirectionPhiFilter);
DEFINE_GAMOS_FILTER(GmPositionPreXFilter);
DEFINE_GAMOS_FILTER(GmPositionPreYFilter);
DEFINE_GAMOS_FILTER(GmPositionPreZFilter);
DEFINE_GAMOS_FILTER(GmDirectionPreThetaFilter);
DEFINE_GAMOS_FILTER(GmDirectionPrePhiFilter);

DEFINE_GAMOS_FILTER(GmProdCutOutsideVoxelFilter);
DEFINE_GAMOS_FILTER(GmProdCutAllProcOutsideVoxelFilter);
DEFINE_GAMOS_FILTER(GmMinRangeCutOutsideVoxelFilter);

DEFINE_GAMOS_FILTER(GmORFilter);
DEFINE_GAMOS_FILTER(GmXORFilter);
DEFINE_GAMOS_FILTER(GmANDFilter);
DEFINE_GAMOS_FILTER(GmOnSecondaryFilter);
DEFINE_GAMOS_FILTER(GmPrimaryOnAllSecondariesFilter);
DEFINE_GAMOS_FILTER(GmPrimaryOn1SecondaryFilter);
DEFINE_GAMOS_FILTER(GmHistoryFilter);
DEFINE_GAMOS_FILTER(GmPastHistoryFilter);
DEFINE_GAMOS_FILTER(GmHistoryOrAncestorsFilter);
DEFINE_GAMOS_FILTER(GmHistoryOrAncestorsFilterForScorer);
DEFINE_GAMOS_FILTER(GmHistoryAllFilter);
DEFINE_GAMOS_FILTER(GmHistoryAncestorsAllFilter);
DEFINE_GAMOS_FILTER(GmAncestorsFilter);
DEFINE_GAMOS_FILTER(GmParentFilter);
DEFINE_GAMOS_FILTER(GmInverseFilter);

DEFINE_GAMOS_FILTER(GmFilterFromClassifier);

#include "GamosCore/GamosBase/Base/include/GmVerbosityFactory.hh"
#include "GmFilterVerbosity.hh"
DEFINE_SEAL_PLUGIN(GmVerbosityFactory, GmFilterVerbosity, "GmFilterVerbosity");

#endif
