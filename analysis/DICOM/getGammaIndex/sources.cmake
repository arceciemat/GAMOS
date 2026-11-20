# - getGammaIndex module build definition

gamos_add_module(getGammaIndex
  PUBLIC_HEADERS
    DCM2DOperGammaIndex.hh
    DCMGetGammaIndex.hh
  SOURCES
    DCM2DOperGammaIndex.cc
    DCMGetGammaIndex.cc
)

gamos_module_link_libraries(getGammaIndex
  PUBLIC
    DICOMBase
    DICOMReaders
    GamosCore_GamosBase_Base
    GamosCore_GamosUtils
)

# Program getGammaIndex
gamos_add_executable(getGammaIndex
  SOURCES
    getGammaIndex.cc
  LINK_LIBRARIES
    getGammaIndex
)
