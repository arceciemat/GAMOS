#------------------------------------------------------------------------------
# Module : GamosUtils
# Package: GamosCore_GamosUtils
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
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosUtils
	 HEADERS
		GmVerbosity.hh
		GmGenUtils.hh
		GmCylindricalSurface.hh
		GmG4Utils.hh
		GmLine.hh
		GmNumberOfEvent.hh
		GmFileIn.hh
		GmPlane.hh
		GmMovementUtils.hh
		GmCone.hh
	 SOURCES
		GmLine.cc
		GmNumberOfEvent.cc
		GmCone.cc
		GmMovementUtils.cc
		GmCylindricalSurface.cc
		GmVerbosity.cc
		GmG4Utils.cc
		GmPlane.cc
		GmFileIn.cc
		GmGenUtils.cc
	 
	 GRANULAR_DEPENDENCIES
	 GLOBAL_DEPENDENCIES
		${Geant4_libs} 
		${ROOT_LIBRARIES} 
		${Boost_LIBRARIES} 

	LINK_LIBRARIES
)
