# - sqdoseToInterfile module build definition

gamos_add_module(sqdoseToInterfile
  SOURCES
    dummy.cc
)

gamos_module_link_libraries(sqdoseToInterfile
  PUBLIC
    GamosCore_GamosReadDICOM
)

# Program sqdoseToInterfile
gamos_add_executable(sqdoseToInterfile
  SOURCES
    sqdoseToInterfile.cc
  LINK_LIBRARIES
    sqdoseToInterfile
    GamosCore_GamosReadDICOM
)
