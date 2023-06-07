# - GAMOS_analysis_DICOM_fmjpeg2koj_dcmcjp2k module build definition

include(UseGamosAtGeant4)

geant4_add_module(GAMOS_analysis_DICOM_fmjpeg2koj_dcmcjp2k
  PUBLIC_HEADERS
    fmjpeg2k
  SOURCES
    djutils.cc
    memory_file.cc
    djcodecd.cc
    djcodece.cc
    djencode.cc
    djrparam.cc
    djcparam.cc
    djdecode.cc
)
geant4_module_link_libraries(GAMOS_analysis_DICOM_fmjpeg2koj_dcmcjp2k
  PUBLIC
    ${Geant4_libs}
    ${ROOT_LIBRARIES}
    XTRALIBS
    tiff
    ${SEAL_LIBRARIES} 
)
