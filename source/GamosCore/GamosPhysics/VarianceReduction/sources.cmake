#------------------------------------------------------------------------------
# Module : VarianceReduction
# Package: GamosCore_GamosPhysics_VarianceReduction
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
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosPhysics_VarianceReduction
	 HEADERS
		GmVarianceReductionMessenger.hh
		GmPhysicsGammaPenelopeDeexSplit.hh
		GmImportanceSamplingProcess.hh
		GmDeexSplitLivermorePhotoElectricModel.hh
		GmVDeexSplitEnergyLossProcess.hh
		GmWrapperProcess.hh
		GmSplitWrapperProcess.hh
		GmDeexSplitPenelopeComptonModel.hh
		GmInheritTrackInfoUA.hh
		GmPhysicsGammaLowEnerDeexSplit.hh
		GmPhysicsElectronPenelopeDeexSplit.hh
		GmDeexSplitPenelopePhotoElectricModel.hh
		GmPhysicsElectronLowEnerUBSDeexSplit.hh
		GmPhysicsElectronLowEnerDeexSplit.hh
		GmDeexSplitLivermoreComptonModel.hh
		GmDeexSpliteIonisation.hh
	 SOURCES
		GmImportanceSamplingProcess.cc
		GmWrapperProcess.cc
		GmSplitWrapperProcess.cc
		GmVarianceReductionMessenger.cc
		GmInheritTrackInfoUA.cc
	 
	 GRANULAR_DEPENDENCIES
	 GLOBAL_DEPENDENCIES
		${Geant4_libs} 
		${ROOT_LIBRARIES} 
	GamosCore_GamosUtils
	GamosCore_GamosGeometry
	GamosCore_GamosPhysics_PhysicsList
	GamosCore_GamosUserActionMgr
	GamosCore_GamosUtils
		${SEAL_LIBRARIES} 

	LINK_LIBRARIES
)
