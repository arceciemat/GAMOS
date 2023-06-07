# - GamosCore_GamosMovement module build definition

include(UseGamosAtGeant4)
include(UseROOT)

geant4_add_module(GamosCore_GamosMovement
  PUBLIC_HEADERS
    GmMovementNEvents.hh
    GmMovementFromFile.hh
    GmMovementVerbosity.hh
    GmVMovement.hh
    GmMovementMgr.hh
    GmMovementRotation.hh
    GmMovementTime.hh
    GmMovementDisplacement.hh
    GmMovementMessenger.hh
    GmMovementEventAction.hh
  SOURCES
    GmMovementFromFile.cc
    GmMovementVerbosity.cc
    plugin.cc
    GmMovementMgr.cc
    GmMovementRotation.cc
    GmVMovement.cc
    GmMovementMessenger.cc
    GmMovementDisplacement.cc
    GmMovementEventAction.cc
)
# - Add path to generated header
geant4_module_include_directories(GamosCore_GamosMovement
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/SealBase/include>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/SealPlatform/include>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/PluginManager/include>
 PUBLIC  $<BUILD_INTERFACE:${ROOT_BASE_DIR}/include>)

geant4_module_link_libraries(GamosCore_GamosMovement
  PUBLIC
    ${Geant4_libs}
    ${ROOT_LIBRARIES}
    ${SEAL_LIBRARIES} 
    GamosCore_GamosUtils
    GamosCore_GamosUserActionMgr
    GamosCore_GamosGeometry
    GamosCore_GamosGenerator
)
