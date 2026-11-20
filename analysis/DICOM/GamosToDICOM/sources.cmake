# - GamosToDICOM module build definition

gamos_add_module(GamosToDICOM
  PUBLIC_HEADERS
    DCMGamosToDICOM.hh
  SOURCES
    DCMGamosToDICOM.cc
)

gamos_module_link_libraries(GamosToDICOM
  PUBLIC
    DICOMBase
    DICOMReaders
    GamosCore_GamosBase_Base
    GamosCore_GamosUtils
    ${GAMOS_DCMTK_LIBRARIES}
)

# Program GamosToDICOM
gamos_add_executable(GamosToDICOM
  SOURCES
    GamosToDICOM.cc
  LINK_LIBRARIES
    GamosToDICOM
)
