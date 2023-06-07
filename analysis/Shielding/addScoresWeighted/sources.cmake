# - GAMOS_analysis_Shielding_addScoresWeighted module build definition

include(UseGamosAtGeant4)

geant4_add_module(GAMOS_analysis_Shielding_addScoresWeighted
  PUBLIC_HEADERS
    ScoreSet.hh
  SOURCES
    ScoreSet.cc
)
geant4_module_link_libraries(GAMOS_analysis_Shielding_addScoresWeighted
  PUBLIC
    ${Geant4_libs}
    ${ROOT_LIBRARIES}
    sumScores
    GamosCore_GamosUtils
    ${SEAL_LIBRARIES} 
)
