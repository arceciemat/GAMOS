# - addScoresWeighted module build definition

gamos_add_module(addScoresWeighted
  PUBLIC_HEADERS
    ScoreSet.hh
  SOURCES
    ScoreSet.cc
)

gamos_module_link_libraries(addScoresWeighted
  PUBLIC
    GamosCore_GamosRunManager
    GamosCore_GamosUtils
    sumScores
)

# Program addScoresWeighted
gamos_add_executable(addScoresWeighted
  SOURCES
    addScoresWeighted.cc
  LINK_LIBRARIES
    addScoresWeighted
    GamosCore_GamosRunManager
    GamosCore_GamosUtils
    sumScores
)
