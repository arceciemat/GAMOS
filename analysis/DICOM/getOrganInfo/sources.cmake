# - getOrganInfo module build definition

gamos_add_module(getOrganInfo
  PUBLIC_HEADERS
    DCMGetOrganInfo.hh
  SOURCES
    DCMGetOrganInfo.cc
)

gamos_module_link_libraries(getOrganInfo
  PUBLIC
    DICOMBase
    DICOMReaders
    GamosCore_GamosBase_Base
    GamosCore_GamosUtils
)

# Program getOrganInfo
gamos_add_executable(getOrganInfo
  SOURCES
    getOrganInfo.cc
  LINK_LIBRARIES
    getOrganInfo
)
