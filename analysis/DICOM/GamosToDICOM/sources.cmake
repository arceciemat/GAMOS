# - GAMOS_analysis_DICOM_GamosToDICOM module build definition

include(UseGamosAtGeant4)

geant4_add_module(GAMOS_analysis_DICOM_GamosToDICOM
  PUBLIC_HEADERS
    DCMGamosToDICOM.hh
    DCMSqToRT.hh.old
  SOURCES
    DCMGamosToDICOM.cc
)
geant4_module_link_libraries(GAMOS_analysis_DICOM_GamosToDICOM
  PUBLIC
    ${Geant4_libs}
    ${ROOT_LIBRARIES}
    DICOMBase
    DICOMReaders
    ${SEAL_LIBRARIES} 
)
