#------------------------------------------------------------------------------
# Module : Detector
# Package: Detector_SOE
#
#------------------------------------------------------------------------------
# 
set(CMAKE_VERBOSE_MAKEFILE ON)
include_directories(${CMAKE_SOURCE_DIR}/source)
include_directories(${CMAKE_SOURCE_DIR}/include)
#
# Define the GAMOS Module.
include(UseGamosAtGeant4)
#
GEANT4_DEFINE_MODULE(NAME SOE
	 HEADERS
set(CMAKE_VERBOSE_MAKEFILE ON)
include_directories(${CMAKE_SOURCE_DIR}/source)
include_directories(${CMAKE_SOURCE_DIR}/include)
#
# Define the GAMOS Module.
include(UseGamosAtGeant4)
#
GEANT4_DEFINE_MODULE(NAME SOE
	 HEADERS
		CDataSetManager.h
		CDensityMatrix.h
		CConeExtension.h
		CHit.h
		CEventManager.h
		CEvent.h
		C3Vector.h
		CCurrentState.h
		constants.h
		CCone.h
		CUserParameters.h
		C3Matrix.h
		CPETLine.h
	 SOURCES
		CConeExtension.cc
		CEvent.cc
		CUserParameters.cc
		CDataSetManager.cc
		constants.cc
		CDensityMatrix.cc
		CHit.cc
		CCone.cc
		CPETLine.cc
		C3Vector.cc
		CEventManager.cc
		C3Matrix.cc
		CCurrentState.cc
	 
	 GRANULAR_DEPENDENCIES
	 GLOBAL_DEPENDENCIES
		${Geant4_LIBRARIES} 
		${GAMOS_LIBRARIES} 
		${SEAL_LIBRARIES} 
		${ROOT_LIBRARIES} 

	LINK_LIBRARIES
)
