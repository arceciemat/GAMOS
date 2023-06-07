# - GamosCore_GamosBase_Classifiers module build definition

include(UseGamosAtGeant4)
include(UseROOT)

geant4_add_module(GamosCore_GamosBase_Classifiers
  PUBLIC_HEADERS
    GmClassifierByPhysicalVolume.hh
    GmClassifierVerbosity.hh
    GmClassifierBy1Ancestor.hh
    GmClassifierByParticleProcess.hh
    GmClassifierByRegion.hh
    GmClassifierByPrimaryParticle.hh
    GmClassifierByParallelLogicalVolume.hh
    GmClassifierBySecondariesListExclude.hh
    GmCompoundClassifier.hh
    GmClassifierByLogicalVolume.hh
    GmClassifierByPhysicalVolumeReplicated.hh
    GmClassifierByHadronicReaction.hh
    GmClassifierByKineticEnergy.hh
    GmClassifierByMaterial.hh
    GmClassifierByAncestors.hh
    GmClassifierByCreatorProcess.hh
    GmClassifierByParticle.hh
    GmClassifierByProcess.hh
    GmClassifierBySecondariesList.hh
  SOURCES
    GmClassifierByCreatorProcess.cc
    GmClassifierByAncestors.cc
    GmClassifierBySecondariesListExclude.cc
    GmClassifierBy1Ancestor.cc
    GmClassifierByParticleProcess.cc
    GmClassifierByPhysicalVolumeReplicated.cc
    GmClassifierByParallelLogicalVolume.cc
    plugin.cc
    GmClassifierByLogicalVolume.cc
    GmClassifierByPhysicalVolume.cc
    GmClassifierByKineticEnergy.cc
    GmClassifierByParticle.cc
    GmClassifierByRegion.cc
    GmClassifierVerbosity.cc
    GmClassifierByPrimaryParticle.cc
    GmClassifierBySecondariesList.cc
    GmClassifierByHadronicReaction.cc
    GmCompoundClassifier.cc
    GmClassifierByProcess.cc
    GmClassifierByMaterial.cc
)
# - Add path to generated header
geant4_module_include_directories(GamosCore_GamosBase_Classifiers
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/SealBase/include>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/SealPlatform/include>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/PluginManager/include>
 PUBLIC  $<BUILD_INTERFACE:${ROOT_BASE_DIR}/include>)

geant4_module_link_libraries(GamosCore_GamosBase_Classifiers
  PUBLIC
    ${Geant4_libs}
    ${ROOT_LIBRARIES}
    ${SEAL_LIBRARIES} 
    GamosCore_GamosUtils
    GamosCore_GamosBase_Base
    GamosCore_GamosGeometry
)
