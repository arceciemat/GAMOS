# - analysePS module build definition

gamos_add_module(analysePS
  SOURCES
    dummy.cc
)

# Program analysePS
gamos_add_executable(analysePS
  SOURCES
    analysePS.cc
  LINK_LIBRARIES
    analysePS
    GamosCore_GamosBase_Base
    GamosCore_GamosRunManager
    GamosCore_GamosUtils
    RadioTherapy
    sumPS
)
