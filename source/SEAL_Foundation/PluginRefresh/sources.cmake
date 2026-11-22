# - SealPluginRefresh module build definition

gamos_add_module(SealPluginRefresh
  SOURCES
    dummy.cc
)

# Program SealPluginRefresh
gamos_add_executable(SealPluginRefresh
  SOURCES
    SealPluginRefresh.cc
  LINK_LIBRARIES
    SealPluginRefresh
    lcg_PluginManager
    lcg_SealBase
)
