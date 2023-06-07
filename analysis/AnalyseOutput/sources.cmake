# - GAMOS_analysis_AnalyseOutput module build definition

include(UseGamosAtGeant4)

geant4_add_module(GAMOS_analysis_AnalyseOutput
  PUBLIC_HEADERS
    GmAOData.hh
    GmAOParam.hh
    GmAODataSearch.hh
  SOURCES
    GmAOData.cc
    GmAODataSearch.cc
)
geant4_module_link_libraries(GAMOS_analysis_AnalyseOutput
  PUBLIC
    ${Geant4_libs}
    ${ROOT_LIBRARIES}
    ${SEAL_LIBRARIES} 
)
