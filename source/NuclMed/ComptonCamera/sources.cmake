# - NuclMed_ComptonCamera module build definition

gamos_add_module(NuclMed_ComptonCamera
  PUBLIC_HEADERS
    CCEventClassifierUA.hh
    CCIOMgr.hh
    CCRecHitSet.hh
    CCVerbosity.hh
  SOURCES
    CCEventClassifierUA.cc
    CCIOMgr.cc
    CCRecHitSet.cc
    CCVerbosity.cc
    plugin.cc
)

gamos_module_link_libraries(NuclMed_ComptonCamera
  PUBLIC
    GamosCore_GamosBase_Base
    GamosCore_GamosSD
    GamosCore_GamosUserActionMgr
    GamosCore_GamosUtils
    NuclMed_Base
    lcg_PluginManager
    lcg_SealBase
)
