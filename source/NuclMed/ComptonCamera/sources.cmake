#------------------------------------------------------------------------------
# Module : ComptonCamera
# Package: NuclMed_ComptonCamera
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
GEANT4_DEFINE_MODULE(NAME NuclMed_ComptonCamera
	 HEADERS
		CCEventClassifierUA.hh
		CCRecHitSet.hh
		CCVerbosity.hh
		CCIOMgr.hh
	 SOURCES
		plugin.cc
		CCEventClassifierUA.cc
		CCRecHitSet.cc
		CCVerbosity.cc
		CCIOMgr.cc
	 
	 GRANULAR_DEPENDENCIES
	 GLOBAL_DEPENDENCIES
		${Geant4_libs} 
		${ROOT_LIBRARIES} 
		${SEAL_LIBRARIES} 
		${Gamos_LIBRARIES} 
		NuclMed_Base

	LINK_LIBRARIES
)
