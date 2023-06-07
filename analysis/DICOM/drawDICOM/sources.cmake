# - GAMOS_analysis_DICOM_drawDICOM module build definition

include(UseGamosAtGeant4)

geant4_add_module(GAMOS_analysis_DICOM_drawDICOM
  PUBLIC_HEADERS
    DCMDrawDICOM.hh
  SOURCES
    DCMDrawDICOM.cc
)
geant4_module_link_libraries(GAMOS_analysis_DICOM_drawDICOM
  PUBLIC
    ${Geant4_libs}
    ${ROOT_LIBRARIES}
    DICOMBase
    DICOM2G4
    DICOMReaders
    ${SEAL_LIBRARIES} 
)
