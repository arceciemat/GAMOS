#------------------------------------------------------------------------------
# Module : Classifiers
# Package: GamosCore_GamosBase_Classifiers
#
#------------------------------------------------------------------------------
#
set(CMAKE_VERBOSE_MAKEFILE ON)
include_directories(${CMAKE_SOURCE_DIR}/source)
include_directories(${CMAKE_SOURCE_DIR}/include)
#
# Define the GEANT4 Module.
include(UseGamosAtGeant4)
#
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosBase_Classifiers
	 HEADERS
		GmClassifierByParticle.hh
		GmClassifierBySecondariesListExclude.hh
		GmClassifierByCreatorProcess.hh
		GmClassifierBy1Ancestor.hh
		GmClassifierByMaterial.hh
		GmClassifierByLogicalVolume.hh
		GmClassifierByPrimaryParticle.hh
		GmClassifierVerbosity.hh
		GmClassifierByProcess.hh
		GmCompoundClassifier.hh
		GmClassifierBySecondariesList.hh
		GmClassifierByPhysicalVolumeReplicated.hh
		GmClassifierByParallelLogicalVolume.hh
		GmClassifierByRegion.hh
		GmClassifierByAncestors.hh
		GmClassifierByHadronicReaction.hh
		GmClassifierByPhysicalVolume.hh
		GmClassifierByKineticEnergy.hh
		GmClassifierByParticleProcess.hh
	 SOURCES
		GmClassifierByParticle.cc
		GmClassifierByPhysicalVolumeReplicated.cc
		GmClassifierByPrimaryParticle.cc
		GmClassifierByLogicalVolume.cc
		plugin.cc
		GmClassifierBySecondariesListExclude.cc
		GmClassifierVerbosity.cc
		GmClassifierByRegion.cc
		GmClassifierByCreatorProcess.cc
		GmClassifierByPhysicalVolume.cc
		GmClassifierByParticleProcess.cc
		GmClassifierByMaterial.cc
		GmClassifierBy1Ancestor.cc
		GmClassifierByParallelLogicalVolume.cc
		GmClassifierBySecondariesList.cc
		GmClassifierByKineticEnergy.cc
		GmClassifierByAncestors.cc
		GmClassifierByProcess.cc
		GmCompoundClassifier.cc
		GmClassifierByHadronicReaction.cc
	 
	 GRANULAR_DEPENDENCIES
	 GLOBAL_DEPENDENCIES
		${Geant4_libs} 
		${ROOT_LIBRARIES} 
	GamosCore_GamosUtils
	GamosCore_GamosBase_Base
	GamosCore_GamosGeometry

	LINK_LIBRARIES
)
