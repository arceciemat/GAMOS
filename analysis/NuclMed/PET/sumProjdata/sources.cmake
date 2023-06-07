# - GAMOS_analysis_NuclMed_PET_sumProjdata module build definition

include(UseGamosAtGeant4)

geant4_add_module(GAMOS_analysis_NuclMed_PET_sumProjdata
  PUBLIC_HEADERS
    sumProjdata.hh
  SOURCES
    dummy.cc
)
geant4_module_link_libraries(GAMOS_analysis_NuclMed_PET_sumProjdata
  PUBLIC
    ${Geant4_libs}
    ${ROOT_LIBRARIES}
    ${SEAL_LIBRARIES} 
)
