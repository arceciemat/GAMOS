#------------------------------------------------------------------------------
# Module : RTDose
# Package: RTDose_analyseSqdose
#
#------------------------------------------------------------------------------
# 
set(CMAKE_VERBOSE_MAKEFILE ON)
include_directories(${CMAKE_SOURCE_DIR}/source)
include_directories(${CMAKE_SOURCE_DIR}/include)
#
# Define the GAMOS Module.
#include(UseGamosAtGeant4)
# 
GEANT4_DEFINE_MODULE(NAME analyseSqdose
	 HEADERS
set(CMAKE_VERBOSE_MAKEFILE ON)
include_directories(${CMAKE_SOURCE_DIR}/source)
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/source/GAMOS620/RadioTherapy)
#
# Define the GAMOS Module.
#include(UseGamosAtGeant4)
#
GEANT4_DEFINE_MODULE(NAME analyseSqdose
	 HEADERS
		Merge3ddoseUA.hh
	 SOURCES
		dummy.cc
	 
	 GRANULAR_DEPENDENCIES
	 GLOBAL_DEPENDENCIES
	#	${Geant4_LIBRARIES} 
		# ${GAMOS_LIBRARIES} 
		${SEAL_LIBRARIES} 
		${ROOT_LIBRARIES} 
	RadioTherapy
	
	LINK_LIBRARIES
)