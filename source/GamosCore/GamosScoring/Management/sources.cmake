# - GamosCore_GamosScoring_Management module build definition

include(UseGamosAtGeant4)
include(UseROOT)

geant4_add_module(GamosCore_GamosScoring_Management
  PUBLIC_HEADERS
    GmEnergySplitter.hh
    GmVPrimitiveScorerVector.hh
    GmScoringRun.hh
    GmVPSPrinter.hh
    GmScoringUA.hh
    GmScoringMgr.hh
    GmPSPrinterMgr.hh
    GmVPrimitiveScorer.hh
    GmScoringVerbosity.hh
    GmCompoundScorer.hh
    GmEnergySplitter.icc
    GmPrimitiveScorerFactory.hh
    GmPSPrinterFactory.hh
    GmScoringMessenger.hh
  SOURCES
    GmScoringRun.cc
    GmPSPrinterFactory.cc
    GmScoringMgr.cc
    GmScoringUA.cc
    GmVPSPrinter.cc
    GmEnergySplitter.cc
    GmPrimitiveScorerFactory.cc
    GmPSPrinterMgr.cc
    plugin.cc
    GmVPrimitiveScorerVector.cc
    GmScoringMessenger.cc
    GmVPrimitiveScorer.cc
    GmCompoundScorer.cc
    GmScoringVerbosity.cc
)
# - Add path to generated header
geant4_module_include_directories(GamosCore_GamosScoring_Management
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/SealBase/include>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/SealPlatform/include>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/PluginManager/include>
 PUBLIC  $<BUILD_INTERFACE:${ROOT_BASE_DIR}/include>)

geant4_module_link_libraries(GamosCore_GamosScoring_Management
  PUBLIC
    ${Geant4_libs}
    ${ROOT_LIBRARIES}
    ${SEAL_LIBRARIES} 
    GamosCore_GamosUtils
    GamosCore_GamosBase_Base
    GamosCore_GamosBase_Filters
    GamosCore_GamosGeometry
    GamosCore_GamosReadDICOM
    GamosCore_GamosGenerator
    GamosCore_GamosData_Management
    GamosCore_GamosData_Users
)
