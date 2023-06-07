# - GAMOS_analysis_RTDose_sqdoseTo3ddose module build definition

include(UseGamosAtGeant4)

geant4_add_module(GAMOS_analysis_RTDose_sqdoseTo3ddose
  PUBLIC_HEADERS
    Merge3ddoseUA.hh
  SOURCES
    dummy.cc
)
geant4_module_link_libraries(GAMOS_analysis_RTDose_sqdoseTo3ddose
  PUBLIC
    ${Geant4_libs}
    ${ROOT_LIBRARIES}
    RadioTherapy
    ${SEAL_LIBRARIES} 
)
