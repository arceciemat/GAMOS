#------------------------------------------------------------------------------
# Module : GamosApplication
# Package: GamosCore_GamosApplication
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
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosApplication
	 HEADERS
	 SOURCES
		dummy.cc
	 
	 GRANULAR_DEPENDENCIES
	 GLOBAL_DEPENDENCIES
		${Geant4_libs} 
		${ROOT_LIBRARIES} 
	MagFieldManager
		${SEAL_LIBRARIES} 

	LINK_LIBRARIES
)
