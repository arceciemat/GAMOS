# - GAMOS_analysis_DICOM_getOrganInfo module build definition

include(UseGamosAtGeant4)

geant4_add_module(GAMOS_analysis_DICOM_getOrganInfo
  PUBLIC_HEADERS
    DCMGetOrganInfo.hh
  SOURCES
    DCMGetOrganInfo.cc
)
geant4_module_link_libraries(GAMOS_analysis_DICOM_getOrganInfo
  PUBLIC
    ${Geant4_libs}
    ${ROOT_LIBRARIES}
    DICOMBase
    DICOMReaders
    ${SEAL_LIBRARIES} 
)
