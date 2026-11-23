# - NuclMed_SPECT module build definition

gamos_add_module(NuclMed_SPECT
  PUBLIC_HEADERS
    SPECTEventClassifierUA.hh
    SPECTIOMgr.hh
    SPECTProjDataMgr.hh
    SPECTVerbosity.hh
  SOURCES
    SPECTEventClassifierUA.cc
    SPECTIOMgr.cc
    SPECTProjDataMgr.cc
    SPECTVerbosity.cc
    plugin.cc
)

gamos_module_link_libraries(NuclMed_SPECT
  PUBLIC
    GamosCore_GamosAnalysis
    GamosCore_GamosBase_Base
    GamosCore_GamosGeometry
    GamosCore_GamosMovement
    GamosCore_GamosSD
    GamosCore_GamosUserActionMgr
    GamosCore_GamosUtils
    NuclMed_Base
    lcg_PluginManager
    lcg_SealBase
)
