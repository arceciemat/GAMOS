# - extendImageAir module build definition

gamos_add_module(extendImageAir
  PUBLIC_HEADERS
    DCMExtendImageAir.hh
  SOURCES
    DCMExtendImageAir.cc
)

gamos_module_link_libraries(extendImageAir
  PUBLIC
    DICOMBase
    DICOMReaders
    GamosCore_GamosReadDICOM
    GamosCore_GamosUtils
)

# Program extendImageAir
gamos_add_executable(extendImageAir
  SOURCES
    extendImageAir.cc
  LINK_LIBRARIES
    extendImageAir
    DICOMBase
    DICOMReaders
    GamosCore_GamosReadDICOM
    GamosCore_GamosUtils
)
