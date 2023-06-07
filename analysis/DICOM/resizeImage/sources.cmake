# - GAMOS_analysis_DICOM_resizeImage module build definition

include(UseGamosAtGeant4)

geant4_add_module(GAMOS_analysis_DICOM_resizeImage
  PUBLIC_HEADERS
    DCMResizeImage.hh
  SOURCES
    DCMResizeImage.cc
)
geant4_module_link_libraries(GAMOS_analysis_DICOM_resizeImage
  PUBLIC
    ${Geant4_libs}
    ${ROOT_LIBRARIES}
    DICOMBase
    DICOMReaders
    DICOM2G4
    ${SEAL_LIBRARIES} 
)
