# - 3ddoseToSqdose module build definition

gamos_add_module(3ddoseToSqdose
  SOURCES
    dummy.cc
)

gamos_module_link_libraries(3ddoseToSqdose
  PUBLIC
    GamosCore_GamosReadDICOM
    GamosCore_GamosUtils
)

# Program 3ddoseToSqdose
gamos_add_executable(3ddoseToSqdose
  SOURCES
    3ddoseToSqdose.cc
  LINK_LIBRARIES
    3ddoseToSqdose
    GamosCore_GamosReadDICOM
    GamosCore_GamosUtils
)
