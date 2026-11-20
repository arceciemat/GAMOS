# - printValueAtPoint module build definition

gamos_add_module(printValueAtPoint
  PUBLIC_HEADERS
    DCMPrintValueAtPoint.hh
  SOURCES
    DCMPrintValueAtPoint.cc
)

gamos_module_link_libraries(printValueAtPoint
  PUBLIC
    DICOMBase
    DICOMReaders
    GamosCore_GamosBase_Base
    GamosCore_GamosUtils
)

# Program printValueAtPoint
gamos_add_executable(printValueAtPoint
  SOURCES
    printValueAtPoint.cc
  LINK_LIBRARIES
    printValueAtPoint
)
