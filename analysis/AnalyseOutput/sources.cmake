#------------------------------------------------------------------------------
# Module : AnalyseOutput
# Package: AnalyseOutput
#
#------------------------------------------------------------------------------
# 
set(CMAKE_VERBOSE_MAKEFILE ON)
include_directories(${CMAKE_SOURCE_DIR}/source)
include_directories(${CMAKE_SOURCE_DIR}/include)
#
set(CMAKE_VERBOSE_MAKEFILE ON)
#include_directories(${PROJECT_SOURCE_DIR}/include)
#
# Define the GAMOS Module.
include(UseGamosAtGeant4)
#
GEANT4_DEFINE_MODULE(NAME AnalyseOutput
	 HEADERS
		GmAODataSearch.hh
		GmAOData.hh
		GmAOParam.hh
	 SOURCES
		GmAOData.cc
		GmAODataSearch.cc
	 
	 GRANULAR_DEPENDENCIES
	 GLOBAL_DEPENDENCIES
		${Geant4_LIBRARIES} 
		${GAMOS_LIBRARIES} 
		${SEAL_LIBRARIES} 
		${ROOT_LIBRARIES} 

	LINK_LIBRARIES
)
