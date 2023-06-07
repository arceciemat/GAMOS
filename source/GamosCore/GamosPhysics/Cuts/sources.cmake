# - GamosCore_GamosPhysics_Cuts module build definition

include(UseGamosAtGeant4)
include(UseROOT)

geant4_add_module(GamosCore_GamosPhysics_Cuts
  PUBLIC_HEADERS
    GmMinRangeLimitsStudyUA.hh
    GmRangeRejectionUA.hh
    GmUserLimitsMessenger.hh
    GmCSTrackStepInfo.hh
    GmStepLimiter.hh
    GmProdCutsForAllProcessesUA.hh
    GmUserSpecialCuts.hh
    GmCutsEnergy2RangeUA.hh
    GmProdCutsStudyUA.hh
    GmCutsEnergy2Range.hh
    GmInitialRangeFilter.hh
    GmMinRangeLimitsStudyCreateInfoUA.hh
    GmUserLimitsMgr.hh
    GmCSTrackInfo.hh
    GmCutsTrackInfo.hh
    GmRangeFilter.hh
    GmCutsStudyMgr.hh
  SOURCES
    GmRangeFilter.cc
    GmInitialRangeFilter.cc
    GmCSTrackInfo.cc
    GmCutsEnergy2Range.cc
    GmRangeRejectionUA.cc
    GmCutsEnergy2RangeUA.cc
    GmCutsTrackInfo.cc
    GmProdCutsStudyUA.cc
    GmMinRangeLimitsStudyCreateInfoUA.cc
    GmCSTrackStepInfo.cc
    GmUserSpecialCuts.cc
    GmCutsStudyMgr.cc
    plugin.cc
    GmStepLimiter.cc
    GmMinRangeLimitsStudyUA.cc
    GmUserLimitsMessenger.cc
    GmUserLimitsMgr.cc
    GmProdCutsForAllProcessesUA.cc
)
# - Add path to generated header
geant4_module_include_directories(GamosCore_GamosPhysics_Cuts
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/SealBase/include>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/SealPlatform/include>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/PluginManager/include>
 PUBLIC  $<BUILD_INTERFACE:${ROOT_BASE_DIR}/include>)

geant4_module_link_libraries(GamosCore_GamosPhysics_Cuts
  PUBLIC
    ${Geant4_libs}
    ${ROOT_LIBRARIES}
    ${SEAL_LIBRARIES} 
    GamosCore_GamosUtils
    GamosCore_GamosGeometry
    GamosCore_GamosPhysics_PhysicsList
)
