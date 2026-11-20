# - compareScores module build definition

gamos_add_module(compareScores
  PUBLIC_HEADERS
    CSVFileData.hh
    CSVScoreData.hh
    CSVScorerData.hh
    FileData.hh
    ROOTFileData.hh
    ROOTScoreData.hh
    ROOTScorerData.hh
    ReadScores.icc
    ScoreData.hh
    ScoreSet.hh
    ScorerData.hh
    TextFileData.hh
    TextScoreData.hh
    TextScorerData.hh
  SOURCES
    CSVFileData.cc
    CSVScoreData.cc
    CSVScorerData.cc
    FileData.cc
    ROOTFileData.cc
    ROOTScoreData.cc
    ROOTScorerData.cc
    ScoreData.cc
    ScoreSet.cc
    ScorerData.cc
    TextFileData.cc
    TextScoreData.cc
    TextScorerData.cc
)

gamos_module_link_libraries(compareScores
  PUBLIC
    GamosCore_GamosBase_Base
    GamosCore_GamosUtils
)

# Program compareScores
gamos_add_executable(compareScores
  SOURCES
    compareScores.cc
  LINK_LIBRARIES
    compareScores
    GamosCore_GamosBase_Base
    GamosCore_GamosRunManager
    GamosCore_GamosUtils
    compareDoses
    sumScores
)
