#------------------------------------------------------------------------------
# Module : Shielding
# Package: Shielding_sumScores
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
GEANT4_DEFINE_MODULE(NAME sumScores
	 HEADERS
set(CMAKE_VERBOSE_MAKEFILE ON)
include_directories(${CMAKE_SOURCE_DIR}/source)
include_directories(${CMAKE_SOURCE_DIR}/include)
#
# Define the GAMOS Module.
include(UseGamosAtGeant4)
#
GEANT4_DEFINE_MODULE(NAME sumScores
	 HEADERS
		TextFileData.hh
		MCNPVTallierData.hh
		MCNPFileData.hh
		GAMOSFileData.hh
		MCNPTallierFNData.hh
		MCNPTallierF5Data.hh
		TextScorerData.hh
		GAMOSScoreData.hh
		GAMOSScorerData.hh
		MCNPTallyData.hh
		FileData.hh
		ScoreData.hh
		ScorerData.hh
		TextScoreData.hh
		ReadScores.icc
	 SOURCES
		MCNPTallierF5Data.cc
		ScoreData.cc
		GAMOSScoreData.cc
		FileData.cc
		ScorerData.cc
		TextScorerData.cc
		TextScoreData.cc
		GAMOSScorerData.cc
		MCNPVTallierData.cc
		MCNPTallierFNData.cc
		TextFileData.cc
		MCNPTallyData.cc
		GAMOSFileData.cc
		MCNPFileData.cc
	 
	 GRANULAR_DEPENDENCIES
	 GLOBAL_DEPENDENCIES
		${Geant4_LIBRARIES} 
		${GAMOS_LIBRARIES} 
		${SEAL_LIBRARIES} 
		${ROOT_LIBRARIES} 

	LINK_LIBRARIES
)
