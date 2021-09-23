#------------------------------------------------------------------------------
# Module : RTDose
# Package: RTDose_compareDoses
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
GEANT4_DEFINE_MODULE(NAME compareDoses
	 HEADERS
		TextFileData.hh
		DoseData.hh
		ROOTDoserData.hh
		ROOTFileData.hh
		DoseSet.hh
		TextDoseData.hh
		FileData.hh
		TextDoserData.hh
		ReadDoses.icc
		DoserData.hh
		ROOTDoseData.hh
	 SOURCES
		FileData.cc
		DoseData.cc
		DoserData.cc
		ROOTFileData.cc
		TextDoserData.cc
		ROOTDoseData.cc
		ROOTDoserData.cc
		DoseSet.cc
		TextFileData.cc
		TextDoseData.cc
	 
	 GRANULAR_DEPENDENCIES
	 GLOBAL_DEPENDENCIES
		${Geant4_LIBRARIES} 
		${GAMOS_LIBRARIES} 
		${SEAL_LIBRARIES} 
		${ROOT_LIBRARIES} 

	LINK_LIBRARIES
)
