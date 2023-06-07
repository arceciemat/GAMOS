# - GamosCore_GamosUtilsUA module build definition

include(UseGamosAtGeant4)
include(UseROOT)

geant4_add_module(GamosCore_GamosUtilsUA
  PUBLIC_HEADERS
    GmKillAtSteppingActionUA.hh
    GmSaveHistosAtRTCPUA.hh
    GmChangeWeightUA.hh
    GmRandomKillByParticleUA.hh
    GmCountTracksAndStepsUA.hh
    GmRandomByTimeUA.hh
    GmCopyWeightToSecondaryUA.hh
    GmKillAtTrackingActionUA.hh
    GmTimeStudyMgr.hh
    GmTimeStudyUA.hh
    GmMaterialBudgetUA.hh
    GmCountTracksUA.hh
    GmGamosVerboseByEventUA.hh
    GmShowerShapeUA.hh
    GmSSData.hh
    GmKillAtStackingActionUA.hh
    GmGenerHistosUA.hh
    GmSaveHistosAfterNEvents.hh
    GmKillAtTrackingActionRandomUA.hh
    GmVisStoreTrajUA.hh
    GmCountProcessesUA.hh
    GmTimeStudyEventUA.hh
    GmDumpGeometryUA.hh
    GmVisModelByEnergy.hh
    GmKillAtSteppingActionRandomUA.hh
    GmKillAllUA.hh
    GmPrintEMParamsUA.hh
    GmKillAtStackingActionRandomUA.hh
    GmKillTooManyStepsUA.hh
    GmPrintXSUA.hh
    GmKillAtSteppkingActionRandomUA.hh
    GmUtilsUAVerbosity.hh
    GmSaveStepUA.hh
    GmVisModelUA.hh
    GmCopyWeightToRDSecondaryUA.hh
    GmTrackingVerboseUA.hh
    GmStopRunAfterTimeUA.hh
  SOURCES
    GmTimeStudyUA.cc
    GmKillAtSteppingActionUA.cc
    GmKillAtTrackingActionRandomUA.cc
    GmMaterialBudgetUA.cc
    GmCopyWeightToRDSecondaryUA.cc
    GmPrintEMParamsUA.cc
    GmCopyWeightToSecondaryUA.cc
    GmStopRunAfterTimeUA.cc
    GmChangeWeightUA.cc
    GmSaveHistosAfterNEvents.cc
    GmRandomKillByParticleUA.cc
    GmKillAtSteppingActionRandomUA.cc
    GmVisModelByEnergy.cc
    GmUtilsUAVerbosity.cc
    GmKillAtStackingActionRandomUA.cc
    plugin.cc
    GmSSData.cc
    GmGamosVerboseByEventUA.cc
    GmDumpGeometryUA.cc
    GmPrintXSUA.cc
    GmVisStoreTrajUA.cc
    GmKillAtTrackingActionUA.cc
    GmTrackingVerboseUA.cc
    GmVisModelUA.cc
    GmShowerShapeUA.cc
    GmKillAtStackingActionUA.cc
    GmRandomByTimeUA.cc
    GmSaveHistosAtRTCPUA.cc
    GmCountTracksAndStepsUA.cc
    GmTimeStudyMgr.cc
    GmCountProcessesUA.cc
    kk
    GmTimeStudyEventUA.cc
    GmCountTracksUA.cc
    GmKillAllUA.cc
    GmGenerHistosUA.cc
    GmKillTooManyStepsUA.cc
)
# - Add path to generated header
geant4_module_include_directories(GamosCore_GamosUtilsUA
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/SealBase/include>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/SealPlatform/include>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/PluginManager/include>
 PUBLIC  $<BUILD_INTERFACE:${ROOT_BASE_DIR}/include>)

geant4_module_link_libraries(GamosCore_GamosUtilsUA
  PUBLIC
    ${Geant4_libs}
    ${ROOT_LIBRARIES}
    ${SEAL_LIBRARIES} 
    GamosCore_GamosBase_Base
    GamosCore_GamosUserActionMgr
    GamosCore_GamosGeometry
    GamosCore_GamosAnalysis
)
