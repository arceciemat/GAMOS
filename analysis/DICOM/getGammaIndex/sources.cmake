# - GAMOS_analysis_DICOM_getGammaIndex module build definition

include(UseGamosAtGeant4)

geant4_add_module(GAMOS_analysis_DICOM_getGammaIndex
  PUBLIC_HEADERS
    DCM2DOperGammaIndex.hh
    DCMGetGammaIndex.hh
  SOURCES
    DCMGetGammaIndex.cc
    DCM2DOperGammaIndex.cc
)
geant4_module_link_libraries(GAMOS_analysis_DICOM_getGammaIndex
  PUBLIC
    ${Geant4_libs}
    ${ROOT_LIBRARIES}
    DICOMBase
    DICOM2G4
    DICOMReaders
    ${SEAL_LIBRARIES} 
)
