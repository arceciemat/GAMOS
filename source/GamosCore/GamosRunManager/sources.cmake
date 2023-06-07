# - GamosCore_GamosRunManager module build definition

include(UseGamosAtGeant4)
include(UseROOT)

geant4_add_module(GamosCore_GamosRunManager
  PUBLIC_HEADERS
    GmDeprecatedCommandsMessenger.hh
    GmUIMessenger.hh
    GmRunManager.hh
    GmUIterminal.hh
    GmRunMessenger.hh
    GmFactoriesMessenger.hh
  SOURCES
    GmRunManager.cc
    plugin.cc
    GmRunMessenger.cc
    GmUIterminal.cc
    GmUIMessenger.cc
    GmFactoriesMessenger.cc
    GmDeprecatedCommandsMessenger.cc
)
# - Add path to generated header
geant4_module_include_directories(GamosCore_GamosRunManager
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/SealBase/include>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/SealPlatform/include>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/PluginManager/include>
 PUBLIC  $<BUILD_INTERFACE:${ROOT_BASE_DIR}/include>)

geant4_module_link_libraries(GamosCore_GamosRunManager
  PUBLIC
    ${Geant4_libs}
    ${ROOT_LIBRARIES}
    ${SEAL_LIBRARIES} 
    GamosCore_GamosBase_Base
    GamosCore_GamosUtils
    GamosCore_GamosGeometry
    GamosCore_GamosUserActionMgr
    GamosCore_GamosAnalysis
    GamosCore_GamosSD
    GamosCore_GamosScoring_Management
    GamosCore_GamosPhysics_PhysicsList
    GamosCore_GamosPhysics_Cuts
    GamosCore_GamosPhysics_Biasing
    GamosCore_GamosPhysics_VarianceReduction
    MagFieldManager
)
