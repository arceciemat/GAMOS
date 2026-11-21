# - SealPluginDump module build definition

gamos_add_module(SealPluginDump
  SOURCES
    dummy.cc
)

# Program SealPluginDump
# NO_AS_NEEDED: the Geant4 libraries are loaded at start-up; with a multithreaded Geant4 (initial-exec TLS)
# they cannot all be loaded later with dlopen ("cannot allocate memory in static TLS block")
gamos_add_executable(SealPluginDump
  NO_AS_NEEDED
  SOURCES
    SealPluginDump.cc
  LINK_LIBRARIES
    SealPluginDump
    lcg_PluginManager
    lcg_SealBase
)
