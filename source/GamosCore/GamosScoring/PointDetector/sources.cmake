# - GamosCore_GamosScoring_PointDetector module build definition

include(UseGamosAtGeant4)
include(UseROOT)

geant4_add_module(GamosCore_GamosScoring_PointDetector
  PUBLIC_HEADERS
    GmPDSInteractionAngleManager.hh
    GmPDSDetector.hh
    GmPDSGammaProcess.hh
    GmPDSCreateAngleTablesUA.hh
    GmPDSScoringTrackInfo.hh
    GmPDSProcessHelper.hh
    GmPDSScore.hh
    GmPDSOpticalPhotonProcess.hh
    GmPDSNeutronProcess.hh
    GmPDSVProcess.hh
    GmPDSGeantinoProcess.hh
    GmPDSUtils.hh
    GmPDSInvertGeantinoStackUA.hh
    GmPDSUA.hh
  SOURCES
    GmPDSOpticalPhotonProcess.cc
    GmPDSGammaProcess.cc
    GmPDSGeantinoProcess.cc
    GmPDSCreateAngleTablesUA.cc
    GmPDSUA.cc
    plugin.cc
    GmPDSNeutronProcess.cc
    GmPDSInvertGeantinoStackUA.cc
    GmPDSInteractionAngleManager.cc
    GmPDSDetector.cc
    GmPDSVProcess.cc
    GmPDSScore.cc
    GmPDSProcessHelper.cc
    GmPDSUtils.cc
)
# - Add path to generated header
geant4_module_include_directories(GamosCore_GamosScoring_PointDetector
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/SealBase/include>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/SealPlatform/include>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/PluginManager/include>
 PUBLIC  $<BUILD_INTERFACE:${ROOT_BASE_DIR}/include>)

geant4_module_link_libraries(GamosCore_GamosScoring_PointDetector
  PUBLIC
    ${Geant4_libs}
    ${ROOT_LIBRARIES}
    ${SEAL_LIBRARIES} 
    GamosCore_GamosUtils
    GamosCore_GamosBase_Base
    GamosCore_GamosGeometry
    GamosCore_GamosReadDICOM
    GamosCore_GamosGenerator
    GamosCore_GamosData_Management
    GamosCore_GamosData_Users
    GamosCore_GamosScoring_Management
)
