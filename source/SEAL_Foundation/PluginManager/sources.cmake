#------------------------------------------------------------------------------
# 
# Module : SEAL_Foundation_PluginManager
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
GEANT4_DEFINE_MODULE(NAME SEAL_Foundation_PluginManager
	 HEADERS
		PluginFactory.h
		ModuleCache.h
		PluginManager.h
		PluginDef.h
		PluginError.h
		config.h
		MapUniqueIterator.h
		Module.h
		ModuleReg.h
		ModuleDef.h
		PluginInfo.h
		PluginCapabilities.h
		ModuleDescriptor.h
		PluginFactoryBase.h
	 SOURCES
		ModuleCache.cc
		PluginInfo.cc
		PluginParserError.h
		ModuleDescriptor.cc
		ModuleDef.cc
		PluginError.cc
		PluginParserError.cc
		PluginManager.cc
		ModuleReg.cc
		PluginFactoryBase.cc
		Module.cc
		debug.cc
		debug.h
		PluginCapabilities.cc
	 
	 GLOBAL_DEPENDENCIES
		${Geant4_LIBRARIES} 
		${ROOT_LIBRARIES} 
	SEAL_Foundation_SealBase

	LINK_LIBRARIES
)
