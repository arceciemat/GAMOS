#------------------------------------------------------------------------------
# Module : Shielding
# Package: Shielding_compareScores
#
#------------------------------------------------------------------------------
# 
set(CMAKE_VERBOSE_MAKEFILE ON)
include_directories(${CMAKE_SOURCE_DIR}/source)
include_directories(${CMAKE_SOURCE_DIR}/include)
#
# Define the GAMOS Module.
include(UseGamosAtGeant4)
#
GEANT4_DEFINE_MODULE(NAME compareScores
	 HEADERS
		TextFileData.hh
		ScoreSet.hh
		ROOTScoreData.hh
		TextScorerData.hh
		ROOTFileData.hh
		ROOTScorerData.hh
		FileData.hh
		ScoreData.hh
		ScorerData.hh
		TextScoreData.hh
		ReadScores.icc
	 SOURCES
		ScoreData.cc
		ROOTScoreData.cc
		FileData.cc
		ScorerData.cc
		TextScorerData.cc
		ROOTScorerData.cc
		ScoreSet.cc
		TextScoreData.cc
		ROOTFileData.cc
		TextFileData.cc
		#DoseData.cc#
	 
	 GRANULAR_DEPENDENCIES
	 GLOBAL_DEPENDENCIES
		${Geant4_LIBRARIES} 
		${GAMOS_LIBRARIES} 
		${SEAL_LIBRARIES} 
		${ROOT_LIBRARIES} 
	sumScores
	GamosCore_GamosUtils

	LINK_LIBRARIES
)
