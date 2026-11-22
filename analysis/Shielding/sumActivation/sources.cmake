# - sumActivation module build definition

gamos_add_module(sumActivation
  SOURCES
    dummy.cc
)

# Program sumActivation
gamos_add_executable(sumActivation
  SOURCES
    sumActivation.cc
  LINK_LIBRARIES
    sumActivation
    GamosCore_GamosRunManager
    GamosCore_GamosUtils
    Shielding
)
