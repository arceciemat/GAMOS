#------------------------------------------------------------------------------
# Module : Management
# Package: GamosCore_GamosScoring_Management
#
#------------------------------------------------------------------------------
#
set(CMAKE_VERBOSE_MAKEFILE ON)
include_directories(${CMAKE_SOURCE_DIR}/source)
include_directories(${CMAKE_SOURCE_DIR}/include)
#
# Define the GEANT4 Module.
include(UseGamosAtGeant4)
#
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosScoring_Management
	 HEADERS
		GmPrimitiveScorerFactory.hh
		GmVChangeWorldAndSource.hh
		GmScoringMessenger.hh
		GmVPSPrinter.hh
		GmVPrimitiveScorer.hh
		GmScoringVerbosity.hh
		GmEnergySplitter.icc
		GmPSPrinterFactory.hh
		GmPSPrinterMgr.hh
		GmVPrimitiveScorerVector.hh
		GmScoringRun.hh
		GmEnergySplitter.hh
		GmScoringUA.hh
		GmCompoundScorer.hh
		GmScoringMgr.hh
	 SOURCES
		GmPSPrinterMgr.cc
		plugin.cc
		GmScoringUA.cc
		GmVChangeWorldAndSource.cc
		GmEnergySplitter.cc
		GmCompoundScorer.cc
		GmVPrimitiveScorerVector.cc
		GmVPrimitiveScorer.cc
		GmScoringVerbosity.cc
		GmScoringRun.cc
		GmScoringMgr.cc
		GmPrimitiveScorerFactory.cc
		GmPSPrinterFactory.cc
		GmVPSPrinter.cc
		GmScoringMessenger.cc
	 
	 GRANULAR_DEPENDENCIES
	 GLOBAL_DEPENDENCIES
		${Geant4_libs} 
		${ROOT_LIBRARIES} 
	GamosCore_GamosUtils
	GamosCore_GamosBase_Base
	GamosCore_GamosBase_Filters
	GamosCore_GamosGeometry
	GamosCore_GamosReadDICOM
	GamosCore_GamosGenerator
	GamosCore_GamosData_Management
		${SEAL_LIBRARIES} 

	LINK_LIBRARIES
)
