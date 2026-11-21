# - sqdoseToRTDose module build definition

gamos_add_module(sqdoseToRTDose
  PUBLIC_HEADERS
    DCMSqToRT.hh
  SOURCES
    DCMSqToRT.cc
)

gamos_module_link_libraries(sqdoseToRTDose
  PUBLIC
    DICOMBase
    DICOMReaders
    GamosCore_GamosBase_Base
    GamosCore_GamosUtils
    ${GAMOS_DCMTK_LIBRARIES}
)

# Program sqdoseToRTDose
gamos_add_executable(sqdoseToRTDose
  SOURCES
    sqdoseToRTDose.cc
  LINK_LIBRARIES
    sqdoseToRTDose
)
