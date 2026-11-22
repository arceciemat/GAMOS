# - changeExtraStructures module build definition

gamos_add_module(changeExtraStructures
  PUBLIC_HEADERS
    DCMChangeExtraStructures.hh
  SOURCES
    DCMChangeExtraStructures.cc
)

gamos_module_link_libraries(changeExtraStructures
  PUBLIC
    DICOMBase
    DICOMReaders
    GamosCore_GamosUtils
)

# Program changeExtraStructures
gamos_add_executable(changeExtraStructures
  SOURCES
    changeExtraStructures.cc
  LINK_LIBRARIES
    changeExtraStructures
    DICOMBase
    DICOMReaders
    GamosCore_GamosUtils
)
