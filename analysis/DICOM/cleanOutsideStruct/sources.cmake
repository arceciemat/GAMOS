# - cleanOutsideStruct module build definition

gamos_add_module(cleanOutsideStruct
  PUBLIC_HEADERS
    DCMCleanOutsideStruct.hh
  SOURCES
    DCMCleanOutsideStruct.cc
)

gamos_module_link_libraries(cleanOutsideStruct
  PUBLIC
    DICOMBase
    DICOMReaders
    GamosCore_GamosUtils
)

# Program cleanOutsideStruct
gamos_add_executable(cleanOutsideStruct
  SOURCES
    cleanOutsideStruct.cc
  LINK_LIBRARIES
    cleanOutsideStruct
    DICOMBase
    DICOMReaders
    GamosCore_GamosUtils
)
