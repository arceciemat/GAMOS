# - changeStructure module build definition

gamos_add_module(changeStructure
  PUBLIC_HEADERS
    DCMChangeStructure.hh
  SOURCES
    DCMChangeStructure.cc
)

gamos_module_link_libraries(changeStructure
  PUBLIC
    DICOMBase
    DICOMReaders
    GamosCore_GamosBase_Base
    GamosCore_GamosUtils
)

# Program changeStructure
gamos_add_executable(changeStructure
  SOURCES
    changeStructure.cc
  LINK_LIBRARIES
    changeStructure
)
