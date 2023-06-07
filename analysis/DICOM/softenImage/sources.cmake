# - GAMOS_analysis_DICOM_softenImage module build definition

include(UseGamosAtGeant4)

geant4_add_module(GAMOS_analysis_DICOM_softenImage
  PUBLIC_HEADERS
    DCMSoftenImage.hh
  SOURCES
    DCMSoftenImage.cc
)
geant4_module_link_libraries(GAMOS_analysis_DICOM_softenImage
  PUBLIC
    ${Geant4_libs}
    ${ROOT_LIBRARIES}
    DICOMBase
    DICOMReaders
    DICOM2G4
    ${SEAL_LIBRARIES} 
)
