# - sum3ddose module build definition

gamos_add_module(sum3ddose
  PUBLIC_HEADERS
    Merge3ddoseUA.hh
  SOURCES
    dummy.cc
)

# Program sum3ddose
gamos_add_executable(sum3ddose
  SOURCES
    sum3ddose.cc
  LINK_LIBRARIES
    sum3ddose
    GamosCore_GamosReadDICOM
    GamosCore_GamosUtils
)
