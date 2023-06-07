# - GAMOS_analysis_DICOM_printValueAtPoint module build definition

include(UseGamosAtGeant4)

geant4_add_module(GAMOS_analysis_DICOM_printValueAtPoint
  PUBLIC_HEADERS
    DCMPrintValueAtPoint.hh
  SOURCES
    DCMPrintValueAtPoint.cc
)
geant4_module_link_libraries(GAMOS_analysis_DICOM_printValueAtPoint
  PUBLIC
    ${Geant4_libs}
    ${ROOT_LIBRARIES}
    DICOMBase
    DICOM2G4
    DICOMReaders
    ${SEAL_LIBRARIES} 
)
