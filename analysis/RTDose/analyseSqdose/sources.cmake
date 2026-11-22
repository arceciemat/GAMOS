# - analyseSqdose module build definition

gamos_add_module(analyseSqdose
  PUBLIC_HEADERS
    Merge3ddoseUA.hh
  SOURCES
    dummy.cc
)

# Program analyseSqdose
gamos_add_executable(analyseSqdose
  SOURCES
    analyseSqdose.cc
  LINK_LIBRARIES
    analyseSqdose
    GamosCore_GamosBase_Base
    GamosCore_GamosReadDICOM
    GamosCore_GamosRunManager
    GamosCore_GamosScoring_Management
    GamosCore_GamosScoring_Scorers
    GamosCore_GamosUtils
    RadioTherapy
)
