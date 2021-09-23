#------------------------------------------------------------------------------
# Module : ghadd
# Package: ROOTUtilities_ghadd
#
#------------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)
include(GamosMacroLibraryTargets)
GAMOS_GLOBAL_LIBRARY_TARGET(COMPONENTS sources.cmake)
	${SEAL_LIBRARIES} 
	${DCMTK_LIBRARIES} 
set(CMAKE_VERBOSE_MAKEFILE ON)
include_directories(${CMAKE_SOURCE_DIR}/source)
include_directories(${CMAKE_SOURCE_DIR}/include)
#
# Define the GAMOS Module.
include(UseGamosAtGeant4)
#
GEANT4_DEFINE_MODULE(NAME ghadd
	 HEADERS
		TFileMerger.hh
	 SOURCES
		TFileMerger.cc
		dummy.cc
	 
	 GRANULAR_DEPENDENCIES
	 GLOBAL_DEPENDENCIES
		${Geant4_LIBRARIES} 
		${GAMOS_LIBRARIES} 
		${SEAL_LIBRARIES} 
		${ROOT_LIBRARIES} 
		${DCMTK_LIBRARIES} 

	LINK_LIBRARIES
)
