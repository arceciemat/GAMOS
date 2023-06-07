# - GAMOS_analysis_Shielding_compareScores module build definition

include(UseGamosAtGeant4)

geant4_add_module(GAMOS_analysis_Shielding_compareScores
  PUBLIC_HEADERS
    ReadScores.icc
    FileData.hh
    TextScorerData.hh
    ROOTScoreData.hh
    TextFileData.hh
    ROOTFileData.hh
    ScoreData.hh
    ScorerData.hh
    ROOTScorerData.hh
    TextScoreData.hh
    ScoreSet.hh
  SOURCES
    TextScoreData.cc
    ScoreData.cc
    ScorerData.cc
    ScoreSet.cc
    ROOTScoreData.cc
    TextScorerData.cc
    ROOTFileData.cc
    ROOTScorerData.cc
    FileData.cc
    TextFileData.cc
)
geant4_module_link_libraries(GAMOS_analysis_Shielding_compareScores
  PUBLIC
    ${Geant4_libs}
    ${ROOT_LIBRARIES}
    sumScores
    GamosCore_GamosUtils
    ${SEAL_LIBRARIES} 
)
