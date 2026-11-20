# - getImageValue module build definition

gamos_add_module(getImageValue
  PUBLIC_HEADERS
    DCMGetImageValue.hh
  SOURCES
    DCMGetImageValue.cc
)

gamos_module_link_libraries(getImageValue
  PUBLIC
    DICOMBase
    DICOMReaders
    GamosCore_GamosBase_Base
    GamosCore_GamosUtils
)

# Program getImageValue
gamos_add_executable(getImageValue
  SOURCES
    getImageValue.cc
  LINK_LIBRARIES
    getImageValue
)
