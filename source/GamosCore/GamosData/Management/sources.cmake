# - GamosCore_GamosData_Management module build definition

include(UseGamosAtGeant4)
include(UseROOT)

geant4_add_module(GamosCore_GamosData_Management
  PUBLIC_HEADERS
    GmDataMgr.hh
    GmVDataAccumulated.hh
    GmVDataProf1DHisto.hh
    GmVDataString.hh
    GmVDataInt.hh
    GmVData2DHisto.hh
    GmDataAncestorStepStringData.hh
    GmVData.hh
    GmDataAncestorStepNumericData.hh
    GmVDataNon1DHisto.hh
    GmVDataProf2DHisto.hh
    GmDataVerbosity.hh
    GmDataFactory.hh
  SOURCES
    GmDataFactory.cc
    GmDataVerbosity.cc
    GmVData2DHisto.cc
    plugin.cc
    GmVData.cc
    GmVDataProf1DHisto.cc
    GmDataAncestorStepNumericData.cc
    GmDataAncestorStepStringData.cc
    GmVDataAccumulated.cc
    GmVDataProf2DHisto.cc
    GmVDataString.cc
    GmDataMgr.cc
    GmVDataNon1DHisto.cc
    GmVDataInt.cc
)
# - Add path to generated header
geant4_module_include_directories(GamosCore_GamosData_Management
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/SealBase/include>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/SealPlatform/include>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/PluginManager/include>
 PUBLIC  $<BUILD_INTERFACE:${ROOT_BASE_DIR}/include>)

geant4_module_link_libraries(GamosCore_GamosData_Management
  PUBLIC
    ${Geant4_libs}
    ${ROOT_LIBRARIES}
    ${SEAL_LIBRARIES} 
    GamosCore_GamosBase_Base
    GamosCore_GamosUserActionMgr
)
