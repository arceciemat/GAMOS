# - GamosCore_GamosAnalysis module build definition

include(UseGamosAtGeant4)
include(UseROOT)

geant4_add_module(GamosCore_GamosAnalysis
  PUBLIC_HEADERS
    GmVTrajStep.hh
    GmTrajPoint.hh
    GmTrajectorySteps.hh
    GmTrajPointProcess.hh
    GmVEventClassifier.hh
    GmTrajectoryPosMom.hh
    GmTrajPointSeco.hh
    GmTrajStep.hh
    GmTrajectory.hh
    GmAnalysisVerbosity.hh
    GmCheckOriginalGamma.hh
    GmTrajStepProcess.hh
    GmVTrajPoint.hh
    GmVTrajectory.hh
    GmVHistoMgr.hh
    GmEventClassifierByGammaInteraction.hh
    GmTrajStepEner.hh
    GmVHistoBuilder.hh
    GmTrajStepPosMom.hh
    GmTrajPointPosMom.hh
  SOURCES
    GmAnalysisVerbosity.cc
    GmTrajPointPosMom.cc
    GmTrajStep.cc
    plugin.cc
    GmTrajPointProcess.cc
    GmTrajStepProcess.cc
    GmTrajPointSeco.cc
    GmTrajectory.cc
    GmCheckOriginalGamma.cc
    GmTrajPoint.cc
    GmTrajStepEner.cc
    GmTrajStepPosMom.cc
    GmVTrajectory.cc
    GmVHistoBuilder.cc
    GmTrajectoryPosMom.cc
    GmTrajectorySteps.cc
    GmEventClassifierByGammaInteraction.cc
)
# - Add path to generated header
geant4_module_include_directories(GamosCore_GamosAnalysis
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/SealBase/include>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/SealPlatform/include>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/PluginManager/include>
 PUBLIC  $<BUILD_INTERFACE:${ROOT_BASE_DIR}/include>)

geant4_module_link_libraries(GamosCore_GamosAnalysis
  PUBLIC
    ${Geant4_libs}
    ${ROOT_LIBRARIES}
    ${SEAL_LIBRARIES} 
    GamosCore_GamosUtils
    GamosCore_GamosBase_Base
    GamosCore_GamosUserActionMgr
)
