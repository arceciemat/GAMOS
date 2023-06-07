# - GamosCore_GamosPhysics_Biasing module build definition

include(UseGamosAtGeant4)
include(UseROOT)

geant4_add_module(GamosCore_GamosPhysics_Biasing
  PUBLIC_HEADERS
    GmBOptnEWRayleigh.hh
    GmBOptrForceCollision.hh
    GmBOptnBremsSplitting.hh
    GmBOptnEWBS.hh
    GmEWBSSplittingProcess.hh
    GmBOptnEWCompton.hh
    export.lis
    GmBOptnDirBremsSplitting.hh
    GmVBiasingOperator.hh
    GmVEWBiasingOperation.hh
    GmBiasingVerbosity.hh
    GmBOptnEWPhotoElectric.hh
    GmBOptrChangeCrossSectionByDistribution.hh
    GmBiasingOperatorFactory.hh
    GmBOptnEWAnnihilation.hh
    GmBOptnEWBremsstrahlung.hh
    GmVEWBiasingOperation.h
    GmBOptrChangeCrossSection.hh
    GmBOptrForceCollisionTrackData.hh
    GmBiasingMessenger.hh
    GmBOptrDirBremsSplitting.hh
    GmGenericBiasingPhysics.hh
    GmBOptrEWBS.hh
    GmBOptrBremsSplitting.hh
    GmBOptnEWPairProduction.hh
    GmBiasingMgr.hh
  SOURCES
    GmBiasingMgr.cc
    GmBOptrDirBremsSplitting.cc
    GmBOptnEWPairProduction.cc
    GmBOptrForceCollisionTrackData.cc
    GmBOptnBremsSplitting.cc
    GmGenericBiasingPhysics.cc
    GmBOptrEWBS.cc
    GmBiasingMessenger.cc
    GmBOptnEWAnnihilation.cc
    export.lis
    GmBOptrBremsSplitting.cc
    GmBOptrChangeCrossSection.cc
    GmBOptrForceCollision.cc
    plugin.cc
    GmVEWBiasingOperation.cc
    GmBOptnEWBS.cc
    GmBOptnEWCompton.cc
    GmBiasingOperatorFactory.cc
    GmEWBSSplittingProcess.cc
    GmBOptnEWPhotoElectric.cc
    GmBOptnEWBremsstrahlung.cc
    GmBOptrChangeCrossSectionByDistribution.cc
    GmBiasingVerbosity.cc
    GmBOptnEWRayleigh.cc
    GmBOptnDirBremsSplitting.cc
    GmVBiasingOperator.cc
)
# - Add path to generated header
geant4_module_include_directories(GamosCore_GamosPhysics_Biasing
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/SealBase/include>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/SealPlatform/include>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/PluginManager/include>
 PUBLIC  $<BUILD_INTERFACE:${ROOT_BASE_DIR}/include>)

geant4_module_link_libraries(GamosCore_GamosPhysics_Biasing
  PUBLIC
    ${Geant4_libs}
    ${ROOT_LIBRARIES}
    ${SEAL_LIBRARIES} 
    GamosCore_GamosUtils
    GamosCore_GamosGeometry
    GamosCore_GamosPhysics_PhysicsList
    GamosCore_GamosUserActionMgr
    GamosCore_GamosUtils
)
