# - sqdoseTo3ddose module build definition

gamos_add_module(sqdoseTo3ddose
  PUBLIC_HEADERS
    Merge3ddoseUA.hh
  SOURCES
    dummy.cc
)

# Program sqdoseTo3ddose
gamos_add_executable(sqdoseTo3ddose
  SOURCES
    sqdoseTo3ddose.cc
  LINK_LIBRARIES
    sqdoseTo3ddose
    GamosCore_GamosReadDICOM
)
