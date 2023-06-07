# - NuclMed_SPECT module build definition

include(UseGamosAtGeant4)
include(UseROOT)

geant4_add_module(NuclMed_SPECT
  PUBLIC_HEADERS
    SPECTVerbosity.hh
    SPECTIOMgr.hh
    SPECTEventClassifierUA.hh
    SPECTProjDataMgr.hh
  SOURCES
    SPECTEventClassifierUA.cc
    SPECTProjDataMgr.cc
    SPECTVerbosity.cc
    plugin.cc
    SPECTIOMgr.cc
)
# - Add path to generated header
geant4_module_include_directories(NuclMed_SPECT
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/SealBase/include>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/SealPlatform/include>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/PluginManager/include>
 PUBLIC  $<BUILD_INTERFACE:${ROOT_BASE_DIR}/include>)

geant4_module_link_libraries(NuclMed_SPECT
  PUBLIC
    ${Geant4_libs}
    ${ROOT_LIBRARIES}
    ${SEAL_LIBRARIES} 
    NuclMed_Base
)
