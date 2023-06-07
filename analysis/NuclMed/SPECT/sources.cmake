# - GAMOS_analysis_NuclMed_SPECT module build definition

include(UseGamosAtGeant4)

geant4_add_module(GAMOS_analysis_NuclMed_SPECT
  PUBLIC_HEADERS
    ASHistoMgr.hh
  SOURCES
    ASHistoMgr.cc
)
geant4_module_link_libraries(GAMOS_analysis_NuclMed_SPECT
  PUBLIC
    ${Geant4_libs}
    ${ROOT_LIBRARIES}
    ${SEAL_LIBRARIES} 
)
