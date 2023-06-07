# - GamosCore_GamosPhysics_OtherPhysicsLists module build definition

include(UseGamosAtGeant4)
include(UseROOT)

geant4_add_module(GamosCore_GamosPhysics_OtherPhysicsLists
  PUBLIC_HEADERS
    GmG4PhysicsLists.hh
    GmQGSP_BIC_HP_EMopt3.icc
    GmDummyPhysics.hh
    GmQGSP_BIC_HP_EMopt3.hh
    GmG4EmDNAPhysics.hh
  SOURCES
    GmG4EmDNAPhysics.cc
    plugin.cc
    GmG4PhysicsLists.cc-
    GmDummyPhysics.cc
)
# - Add path to generated header
geant4_module_include_directories(GamosCore_GamosPhysics_OtherPhysicsLists
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/SealBase/include>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/SealPlatform/include>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/PluginManager/include>
 PUBLIC  $<BUILD_INTERFACE:${ROOT_BASE_DIR}/include>)

geant4_module_link_libraries(GamosCore_GamosPhysics_OtherPhysicsLists
  PUBLIC
    ${Geant4_libs}
    ${ROOT_LIBRARIES}
    ${SEAL_LIBRARIES} 
    GamosCore_GamosUtils
    GamosCore_GamosPhysics_PhysicsList
)
