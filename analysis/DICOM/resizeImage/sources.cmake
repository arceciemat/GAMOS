# - resizeImage module build definition

gamos_add_module(resizeImage
  PUBLIC_HEADERS
    DCMResizeImage.hh
  SOURCES
    DCMResizeImage.cc
)

gamos_module_link_libraries(resizeImage
  PUBLIC
    DICOMBase
    DICOMReaders
    GamosCore_GamosBase_Base
    GamosCore_GamosReadDICOM
    GamosCore_GamosUtils
)

# Program resizeImage
gamos_add_executable(resizeImage
  SOURCES
    resizeImage.cc
  LINK_LIBRARIES
    resizeImage
)
