#------------------------------------------------------------------------------
# Module : Detector
# Package: Detector_PET_lm2pd
#
#------------------------------------------------------------------------------
# 
set(CMAKE_VERBOSE_MAKEFILE ON)
include_directories(${CMAKE_SOURCE_DIR}/source)
include_directories(${PROJECT_SOURCE_DIR}/include)

#
# Define the GAMOS Module.
include(UseGamosAtGeant4)
#
GEANT4_DEFINE_MODULE(NAME lm2pd
	 HEADERS
		lm2pd.hh
	 SOURCES
		dummy.cc
	 
	 GRANULAR_DEPENDENCIES
	 GLOBAL_DEPENDENCIES
		${Geant4_LIBRARIES} 
		${GAMOS_LIBRARIES} 
		${SEAL_LIBRARIES} 
		${ROOT_LIBRARIES} 

	LINK_LIBRARIES
)