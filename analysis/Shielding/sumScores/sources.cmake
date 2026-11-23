# - sumScores module build definition

gamos_add_module(sumScores
  PUBLIC_HEADERS
    FileData.hh
    GAMOSFileData.hh
    GAMOSScoreData.hh
    GAMOSScorerData.hh
    MCNPFileData.hh
    MCNPTallierF5Data.hh
    MCNPTallierFNData.hh
    MCNPTallyData.hh
    MCNPVTallierData.hh
    ReadScores.icc
    ScoreData.hh
    ScorerData.hh
    TextFileData.hh
    TextScoreData.hh
    TextScorerData.hh
  SOURCES
    FileData.cc
    GAMOSFileData.cc
    GAMOSScoreData.cc
    GAMOSScorerData.cc
    MCNPFileData.cc
    MCNPTallierF5Data.cc
    MCNPTallierFNData.cc
    MCNPTallyData.cc
    MCNPVTallierData.cc
    ScoreData.cc
    ScorerData.cc
    TextFileData.cc
    TextScoreData.cc
    TextScorerData.cc
)

gamos_module_link_libraries(sumScores
  PUBLIC
    GamosCore_GamosUtils
)

# Program sumScores
gamos_add_executable(sumScores
  SOURCES
    sumScores.cc
  LINK_LIBRARIES
    sumScores
    GamosCore_GamosRunManager
    GamosCore_GamosUtils
    compareDoses
    compareScores
)
