# - GamosCore_GamosData_Users module build definition

include(UseGamosAtGeant4)
include(UseROOT)

geant4_add_module(GamosCore_GamosData_Users
  PUBLIC_HEADERS
    GmRunDataTextFileUA.hh
    GmStepDataCoutUA.hh
    GmTrackDataBinFileUA.hh
    GmRunDataHistosUA.hh
    GmStackDataHistosUA.hh
    GmTrackDataTextFileUA.hh
    GmSecondaryTrackDataBinFileUA.hh
    GmRunDataBinFileUA.hh
    GmRunDataCoutUA.hh
    GmVDataUserTextFile.hh
    GmClassifierByStringData.hh
    GmVDataUserBinFile.hh
    GmStepDataBinFileUA.hh
    GmClassifierByNumericData.hh
    GmEventDataHistosUA.hh
    GmVDataUser.hh
    GmStringDataFilter.hh
    GmTrackDataHistosUA.hh
    GmSecondaryTrackDataCoutUA.hh
    GmClassifierByDistribution.hh
    GmStackDataTextFileUA.hh
    GmVDataUserHistos.hh
    GmSecondaryTrackDataHistosUA.hh
    GmNumericDataFilter.hh
    GmEventDataCoutUA.hh
    GmTrackDataCoutUA.hh
    GmDataTTreeUA.hh
    GmStepDataTextFileUA.hh
    GmStepDataHistosUA.hh
    GmEventDataTextFileUA.hh
    GmStackDataCoutUA.hh
    GmStackDataBinFileUA.hh
    GmSecondaryTrackDataTextFileUA.hh
    GmEventDataBinFileUA.hh
  SOURCES
    GmEventDataCoutUA.cc
    GmVDataUserBinFile.cc
    GmStackDataCoutUA.cc
    GmTrackDataHistosUA.cc
    GmSecondaryTrackDataBinFileUA.cc
    GmStepDataHistosUA.cc
    GmNumericDataFilter.cc
    GmClassifierByDistribution.cc
    GmTrackDataBinFileUA.cc
    GmSecondaryTrackDataTextFileUA.cc
    GmStackDataTextFileUA.cc
    GmRunDataBinFileUA.cc
    GmSecondaryTrackDataHistosUA.cc
    GmEventDataBinFileUA.cc
    plugin.cc
    GmVDataUser.cc
    GmStepDataCoutUA.cc
    GmSecondaryTrackDataCoutUA.cc
    GmVDataUserTextFile.cc
    GmEventDataTextFileUA.cc
    GmStackDataBinFileUA.cc
    GmTrackDataCoutUA.cc
    GmStackDataHistosUA.cc
    GmEventDataHistosUA.cc
    GmVDataUserHistos.cc
    GmStringDataFilter.cc
    GmDataTTreeUA.cc
    GmStepDataTextFileUA.cc
    GmRunDataHistosUA.cc
    GmClassifierByStringData.cc
    GmClassifierByNumericData.cc
    GmTrackDataTextFileUA.cc
    GmRunDataTextFileUA.cc
    GmRunDataCoutUA.cc
    GmStepDataBinFileUA.cc
)
# - Add path to generated header
geant4_module_include_directories(GamosCore_GamosData_Users
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/SealBase/include>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/SealPlatform/include>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/PluginManager/include>
 PUBLIC  $<BUILD_INTERFACE:${ROOT_BASE_DIR}/include>)

geant4_module_link_libraries(GamosCore_GamosData_Users
  PUBLIC
    ${Geant4_libs}
    ${ROOT_LIBRARIES}
    ${SEAL_LIBRARIES} 
    GamosCore_GamosBase_Base
    GamosCore_GamosData_Management
    GamosCore_GamosAnalysis
    GamosCore_GamosBase_Classifiers
    GamosCore_GamosBase_Filters
)
