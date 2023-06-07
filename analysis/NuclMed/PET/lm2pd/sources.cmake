# - GAMOS_analysis_NuclMed_PET_lm2pd module build definition

include(UseGamosAtGeant4)

geant4_add_module(GAMOS_analysis_NuclMed_PET_lm2pd
  PUBLIC_HEADERS
    lm2pd.hh
  SOURCES
    dummy.cc
)
geant4_module_link_libraries(GAMOS_analysis_NuclMed_PET_lm2pd
  PUBLIC
    ${Geant4_libs}
    ${ROOT_LIBRARIES}
    ${SEAL_LIBRARIES} 
)
