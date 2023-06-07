# - GAMOS_analysis_RTDose_compareDoses module build definition

include(UseGamosAtGeant4)

geant4_add_module(GAMOS_analysis_RTDose_compareDoses
  PUBLIC_HEADERS
    FileData.hh
    ReadDoses.icc
    ROOTDoseData.hh
    TextFileData.hh
    ROOTFileData.hh
    TextDoserData.hh
    ROOTDoserData.hh
    DoserData.hh
    TextDoseData.hh
    DoseSet.hh
    DoseData.hh
  SOURCES
    TextDoseData.cc
    DoseSet.cc
    ROOTDoserData.cc
    DoserData.cc
    ROOTFileData.cc
    TextDoserData.cc
    FileData.cc
    TextFileData.cc
    DoseData.cc
    ROOTDoseData.cc
)
geant4_module_link_libraries(GAMOS_analysis_RTDose_compareDoses
  PUBLIC
    ${Geant4_libs}
    ${ROOT_LIBRARIES}
    ${SEAL_LIBRARIES} 
)
