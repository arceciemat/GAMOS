# - printValueAlongLine module build definition

gamos_add_module(printValueAlongLine
  PUBLIC_HEADERS
    DCMPrintValueAlongLine.hh
  SOURCES
    DCMPrintValueAlongLine.cc
)

gamos_module_link_libraries(printValueAlongLine
  PUBLIC
    DICOMBase
    DICOMReaders
    GamosCore_GamosBase_Base
    GamosCore_GamosUtils
)

# Program printValueAlongLine
gamos_add_executable(printValueAlongLine
  SOURCES
    printValueAlongLine.cc
  LINK_LIBRARIES
    printValueAlongLine
)
