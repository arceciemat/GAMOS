# - sumSqdose module build definition

gamos_add_module(sumSqdose
  PUBLIC_HEADERS
    Merge3ddoseUA.hh
  SOURCES
    dummy.cc
)

# Program sumSqdose
gamos_add_executable(sumSqdose
  SOURCES
    sumSqdose.cc
  LINK_LIBRARIES
    sumSqdose
    GamosCore_GamosReadDICOM
    GamosCore_GamosRunManager
    GamosCore_GamosUtils
)
