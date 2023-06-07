# - GAMOS_analysis_DICOM_changeStructure module build definition

include(UseGamosAtGeant4)

geant4_add_module(GAMOS_analysis_DICOM_changeStructure
  PUBLIC_HEADERS
    DCMChangeStructure.hh
  SOURCES
    DCMChangeStructure.cc
)
geant4_module_link_libraries(GAMOS_analysis_DICOM_changeStructure
  PUBLIC
    ${Geant4_libs}
    ${ROOT_LIBRARIES}
    DICOMBase
    DICOMReaders
    DICOM2G4
    ${SEAL_LIBRARIES} 
)
