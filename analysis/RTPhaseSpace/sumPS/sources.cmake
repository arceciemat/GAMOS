# - sumPS module build definition

gamos_add_module(sumPS
  PUBLIC_HEADERS
    FILE2MEMORY.h
    egsclass.h
    general.h
    histogram.h
    iaea_config.hh
    iaea_header.hh
    iaea_record.hh
    iaea_utilities.hh
    matrix.h
    utilities.h
  SOURCES
    FILE2MEMORY.cc
    egsclass.cc
    general.cc
    histogram.cc
    iaea_header.cc
    iaea_record.cc
    iaea_utilities.cc
    matrix.cc
    utilities.cc
)

gamos_module_link_libraries(sumPS
  PUBLIC
    GamosCore_GamosBase_Base
)

# Program sumPS
gamos_add_executable(sumPS
  SOURCES
    sumPS.cc
  LINK_LIBRARIES
    sumPS
)
