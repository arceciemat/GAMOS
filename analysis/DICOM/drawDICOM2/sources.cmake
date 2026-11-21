# - drawDICOM2 module build definition

gamos_add_module(drawDICOM2
  PUBLIC_HEADERS
    DCMDrawDICOM2.hh
  SOURCES
    DCMDrawDICOM2.cc
)

gamos_module_link_libraries(drawDICOM2
  PUBLIC
    DICOMBase
    DICOMReaders
    GamosCore_GamosBase_Base
    GamosCore_GamosUtils
)

# Program drawDICOM2
gamos_add_executable(drawDICOM2
  SOURCES
    drawDICOM2.cc
  LINK_LIBRARIES
    drawDICOM2
    DICOMBase
    DICOMReaders
    GamosCore_GamosBase_Base
    GamosCore_GamosUtils
)
