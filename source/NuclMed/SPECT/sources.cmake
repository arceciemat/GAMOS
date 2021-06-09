#------------------------------------------------------------------------------
# Module : SPECT
# Package: NuclMed_SPECT
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
GEANT4_DEFINE_MODULE(NAME NuclMed_SPECT
	 HEADERS
		SPECTIOMgr.hh
		SPECTVerbosity.hh
		SPECTEventClassifierUA.hh
	 SOURCES
		plugin.cc
		SPECTIOMgr.cc
		SPECTEventClassifierUA.cc
		SPECTVerbosity.cc
	 
	 GRANULAR_DEPENDENCIES
	 GLOBAL_DEPENDENCIES
		${Geant4_libs} 
		${ROOT_LIBRARIES} 
		${SEAL_LIBRARIES} 
		${Gamos_LIBRARIES} 
		NuclMed_Base

	LINK_LIBRARIES
)
