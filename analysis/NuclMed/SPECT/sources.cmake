# - analyseSPECT module build definition

gamos_add_module(analyseSPECT
  PUBLIC_HEADERS
    ASHistoMgr.hh
  SOURCES
    ASHistoMgr.cc
)

gamos_module_link_libraries(analyseSPECT
  PUBLIC
    GamosCore_GamosAnalysis
    GamosCore_GamosBase_Base
    GamosCore_GamosUtils
)

# Program analyseSPECT
gamos_add_executable(analyseSPECT
  SOURCES
    analyseSPECT.cc
  LINK_LIBRARIES
    analyseSPECT
    GamosCore_GamosBase_Base
    GamosCore_GamosUtils
    NuclMed_SPECT
)
