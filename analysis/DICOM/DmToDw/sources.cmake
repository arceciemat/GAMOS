# - DmToDw module build definition

gamos_add_module(DmToDw
  PUBLIC_HEADERS
    DCMDmToDw.hh
  SOURCES
    DCMDmToDw.cc
)

gamos_module_link_libraries(DmToDw
  PUBLIC
    DICOMBase
    DICOMReaders
    GamosCore_GamosReadDICOM
    GamosCore_GamosUtils
)

# Program DmToDw
gamos_add_executable(DmToDw
  SOURCES
    DmToDw.cc
  LINK_LIBRARIES
    DmToDw
    DICOMBase
    DICOMReaders
    GamosCore_GamosReadDICOM
    GamosCore_GamosUtils
)
