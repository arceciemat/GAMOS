# - lcg_PluginManager module build definition

gamos_add_module(lcg_PluginManager
  PUBLIC_HEADERS
    MapUniqueIterator.h
    Module.h
    ModuleCache.h
    ModuleDef.h
    ModuleDescriptor.h
    ModuleReg.h
    PluginCapabilities.h
    PluginDef.h
    PluginError.h
    PluginFactory.h
    PluginFactoryBase.h
    PluginInfo.h
    PluginManager.h
    config.h
  PRIVATE_HEADERS
    PluginParserError.h
    debug.h
  SOURCES
    Module.cc
    ModuleCache.cc
    ModuleDef.cc
    ModuleDescriptor.cc
    ModuleReg.cc
    PluginCapabilities.cc
    PluginError.cc
    PluginFactoryBase.cc
    PluginInfo.cc
    PluginManager.cc
    PluginParserError.cc
    debug.cc
  HEADER_DIR PluginManager
  HEADER_INSTALL_DIR PluginManager
  NO_GEANT4
)

gamos_module_link_libraries(lcg_PluginManager
  PUBLIC
    lcg_SealBase
    ${CMAKE_DL_LIBS}
    Threads::Threads
)
