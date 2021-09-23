#------------------------------------------------------------------------------
# Module : GamosMovement
# Package: GamosCore_GamosMovement
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
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosMovement
	 HEADERS
		GmVMovement.hh
		GmMovementNEvents.hh
		GmMovementDisplacement.hh
		GmMovementFromFile.hh
		GmMovementRotation.hh
		GmMovementEventAction.hh
		GmMovementTime.hh
		GmMovementMgr.hh
		GmMovementVerbosity.hh
		GmMovementMessenger.hh
	 SOURCES
		plugin.cc
		GmVMovement.cc
		GmMovementFromFile.cc
		GmMovementVerbosity.cc
		GmMovementMgr.cc
		GmMovementDisplacement.cc
		GmMovementRotation.cc
		GmMovementMessenger.cc
		GmMovementEventAction.cc
	 
	 GRANULAR_DEPENDENCIES
	 GLOBAL_DEPENDENCIES
		${Geant4_libs} 
		${ROOT_LIBRARIES} 
	GamosCore_GamosUtils
	GamosCore_GamosUserActionMgr
	GamosCore_GamosGeometry
	GamosCore_GamosGenerator
		${SEAL_LIBRARIES} 

	LINK_LIBRARIES
)
