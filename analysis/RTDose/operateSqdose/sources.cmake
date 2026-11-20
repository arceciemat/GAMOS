# - operateSqdose module build definition

gamos_add_module(operateSqdose
  PUBLIC_HEADERS
    Merge3ddoseUA.hh
  SOURCES
    dummy.cc
)

# Program operateSqdose
gamos_add_executable(operateSqdose
  SOURCES
    operateSqdose.cc
  LINK_LIBRARIES
    operateSqdose
    GamosCore_GamosReadDICOM
    GamosCore_GamosUtils
)
