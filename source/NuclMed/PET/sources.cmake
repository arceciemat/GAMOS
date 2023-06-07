# - NuclMed_PET module build definition

include(UseGamosAtGeant4)
include(UseROOT)

geant4_add_module(NuclMed_PET
  PUBLIC_HEADERS
    PETHistosPositron.hh
    PETEventClassifierTOFUA.hh
    PETIOMgrCrystalID.hh
    PETIOMgr.hh
    PETVComptClassificationVariable.hh
    PETEventClassifierUA.hh
    PETIOMgrTOF.hh
    PETProjDataMgr.hh
    PETVerbosity.hh
    PETPrintPositron.hh
    PETEventClassifierCrystalIDUA.hh
    PETVComptAlgorithmVariable.hh
  SOURCES
    PETEventClassifierUA.cc
    PETPrintPositron.cc
    PETProjDataMgr.cc
    PETHistosPositron.cc
    PETIOMgr.cc
    PETVerbosity.cc
    plugin.cc
    PETEventClassifierTOFUA.cc
    PETEventClassifierCrystalIDUA.cc
    PETIOMgrCrystalID.cc
    PETIOMgrTOF.cc
)
# - Add path to generated header
geant4_module_include_directories(NuclMed_PET
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/SealBase/include>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/SealPlatform/include>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/PluginManager/include>
 PUBLIC  $<BUILD_INTERFACE:${ROOT_BASE_DIR}/include>)

geant4_module_link_libraries(NuclMed_PET
  PUBLIC
    ${Geant4_libs}
    ${ROOT_LIBRARIES}
    ${SEAL_LIBRARIES} 
    NuclMed_Base
)
