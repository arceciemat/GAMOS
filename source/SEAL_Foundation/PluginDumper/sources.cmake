#------------------------------------------------------------------------------
# 
# Module : SEAL_Foundation_PluginDumper
# Package: 
#
#------------------------------------------------------------------------------
set(CMAKE_VERBOSE_MAKEFILE ON)
include(Geant4MacroDefineModule)
include_directories(${CMAKE_SOURCE_DIR}/source)
include_directories(${CMAKE_SOURCE_DIR}/source/SEAL_Foundation/PluginManager)
include_directories(${CMAKE_SOURCE_DIR}/source/SEAL_Foundation/SealPlatform)
include_directories(${CMAKE_SOURCE_DIR}/source/SEAL_Foundation/SealBase)
#
# Define the GAMOS Module.
#
GEANT4_DEFINE_MODULE(NAME SEAL_Foundation_PluginDumper
	 HEADERS
	 SOURCES
		BuildFile
		dummy.cc
	 
	 GLOBAL_DEPENDENCIES
		${Geant4_LIBRARIES} 
		${ROOT_LIBRARIES} 
	SEAL_Foundation_SealBase
	SEAL_Foundation_PluginManager

	LINK_LIBRARIES
)
