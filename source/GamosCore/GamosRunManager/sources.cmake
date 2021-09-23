#------------------------------------------------------------------------------
# Module : GamosRunManager
# Package: GamosCore_GamosRunManager
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
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosRunManager
	 HEADERS
		GmDeprecatedCommandsMessenger.hh
		GmRunManager.hh
		GmFactoriesMessenger.hh
		GmUIMessenger.hh
		GmRunMessenger.hh
		GmUIterminal.hh
	 SOURCES
		plugin.cc
		GmUIterminal.cc
		GmRunMessenger.cc
		GmUIMessenger.cc
		GmDeprecatedCommandsMessenger.cc
		GmFactoriesMessenger.cc
		GmRunManager.cc
	 
	 GRANULAR_DEPENDENCIES
	 GLOBAL_DEPENDENCIES
		${Geant4_libs} 
		${ROOT_LIBRARIES} 
	GamosCore_GamosBase_Base
	GamosCore_GamosUtils
	GamosCore_GamosGeometry
	GamosCore_GamosUserActionMgr
	GamosCore_GamosAnalysis
	GamosCore_GamosSD
	GamosCore_GamosScoring_Management
	GamosCore_GamosPhysics_PhysicsList
	GamosCore_GamosPhysics_Cuts
	GamosCore_GamosPhysics_Biasing
	GamosCore_GamosPhysics_VarianceReduction
	MagFieldManager
		${SEAL_LIBRARIES} 

	LINK_LIBRARIES
)
