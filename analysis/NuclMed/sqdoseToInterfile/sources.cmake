# - GAMOS_analysis_NuclMed_sqdoseToInterfile module build definition

include(UseGamosAtGeant4)

geant4_add_module(GAMOS_analysis_NuclMed_sqdoseToInterfile
  PUBLIC_HEADERS
  SOURCES
    dummy.cc
)
geant4_module_link_libraries(GAMOS_analysis_NuclMed_sqdoseToInterfile
  PUBLIC
    ${Geant4_libs}
    ${ROOT_LIBRARIES}
    ${SEAL_LIBRARIES} 
)
