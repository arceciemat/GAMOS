# - multSqdose module build definition

gamos_add_module(multSqdose
  PUBLIC_HEADERS
    Merge3ddoseUA.hh
  SOURCES
    dummy.cc
)

# Program multSqdose
gamos_add_executable(multSqdose
  SOURCES
    multSqdose.cc
  LINK_LIBRARIES
    multSqdose
    GamosCore_GamosReadDICOM
)
