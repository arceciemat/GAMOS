# - drawDICOM module build definition

gamos_add_module(drawDICOM
  PUBLIC_HEADERS
    DCMDrawDICOM.hh
  SOURCES
    DCMDrawDICOM.cc
)

gamos_module_link_libraries(drawDICOM
  PUBLIC
    DICOMBase
    DICOMReaders
    GamosCore_GamosBase_Base
    GamosCore_GamosUtils
)

# Program drawDICOM
gamos_add_executable(drawDICOM
  SOURCES
    drawDICOM.cc
  LINK_LIBRARIES
    drawDICOM
)
