# - GAMOS_analysis_DICOM_drawDICOM2 module build definition

include(UseGamosAtGeant4)

geant4_add_module(GAMOS_analysis_DICOM_drawDICOM2
  PUBLIC_HEADERS
    DCMDrawDICOM2.hh
  SOURCES
    DCMDrawDICOM2.cc
)
geant4_module_link_libraries(GAMOS_analysis_DICOM_drawDICOM2
  PUBLIC
    ${Geant4_libs}
    ${ROOT_LIBRARIES}
    DICOMBase
    DICOM2G4
    DICOMReaders
    ${SEAL_LIBRARIES} 
)
