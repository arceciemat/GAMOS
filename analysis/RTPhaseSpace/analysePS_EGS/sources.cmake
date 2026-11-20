# - analysePS_EGS module build definition

gamos_add_module(analysePS_EGS
  SOURCES
    dummy.cc
)

# Program analysePS_EGS
gamos_add_executable(analysePS_EGS
  SOURCES
    analysePS_EGS.cc
  LINK_LIBRARIES
    analysePS_EGS
    GamosCore_GamosBase_Base
    GamosCore_GamosUtils
    RadioTherapy
)
