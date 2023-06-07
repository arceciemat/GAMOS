# - GAMOS_analysis_DICOM_printValueAlongLine module build definition

include(UseGamosAtGeant4)

geant4_add_module(GAMOS_analysis_DICOM_printValueAlongLine
  PUBLIC_HEADERS
    DCMPrintValueAlongLine.hh
  SOURCES
    DCMPrintValueAlongLine.cc-
    DCMPrintValueAlongLine.cc
)
geant4_module_link_libraries(GAMOS_analysis_DICOM_printValueAlongLine
  PUBLIC
    ${Geant4_libs}
    ${ROOT_LIBRARIES}
    DICOMBase
    DICOM2G4
    DICOMReaders
    ${SEAL_LIBRARIES} 
)
