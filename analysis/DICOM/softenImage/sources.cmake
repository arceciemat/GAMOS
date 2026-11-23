# - softenImage module build definition

gamos_add_module(softenImage
  PUBLIC_HEADERS
    DCMSoftenImage.hh
  SOURCES
    DCMSoftenImage.cc
)

gamos_module_link_libraries(softenImage
  PUBLIC
    DICOMBase
    DICOMReaders
    GamosCore_GamosReadDICOM
    GamosCore_GamosUtils
)

# Program softenImage
gamos_add_executable(softenImage
  SOURCES
    softenImage.cc
  LINK_LIBRARIES
    softenImage
    DICOMBase
    DICOMReaders
    GamosCore_GamosReadDICOM
    GamosCore_GamosUtils
)
