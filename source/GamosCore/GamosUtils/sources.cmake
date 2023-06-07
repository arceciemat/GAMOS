# - GamosCore_GamosUtils module build definition

include(UseGamosAtGeant4)
include(UseROOT)

geant4_add_module(GamosCore_GamosUtils
  PUBLIC_HEADERS
    GmG4Utils.hh
    GmNumberOfEvent.hh
    GmPlane.hh
    GmGenUtils.hh
    GmCone.hh
    GmMovementUtils.hh
    GmCylindricalSurface.hh
    GmVerbosity.hh
    GmLine.hh
    GmFileIn.hh
  SOURCES
    GmFileIn.cc
    GmVerbosity.cc
    GmPlane.cc
    GmNumberOfEvent.cc
    GmMovementUtils.cc
    GmCone.cc
    GmLine.cc
    GmG4Utils.cc
    GmGenUtils.cc
    GmCylindricalSurface.cc
)
# - Add path to generated header
geant4_module_include_directories(GamosCore_GamosUtils
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/SealBase/include>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/SealPlatform/include>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/PluginManager/include>
 PUBLIC  $<BUILD_INTERFACE:${ROOT_BASE_DIR}/include>)

geant4_module_link_libraries(GamosCore_GamosUtils
  PUBLIC
    ${Geant4_libs}
    ${ROOT_LIBRARIES}
    ${SEAL_LIBRARIES} 
)
