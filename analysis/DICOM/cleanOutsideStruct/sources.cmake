# - GAMOS_analysis_DICOM_changeStructure module build definition

include(UseGamosAtGeant4)

geant4_add_module(GAMOS_analysis_DICOM_changeStructure
  PUBLIC_HEADERS
    DCMCleanOutsideStruct.hh
  SOURCES
    DCMCleanOutsideStruct.cc
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
