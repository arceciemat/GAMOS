#------------------------------------------------------------------------------
# Module : GamosAnalysis
# Package: GamosCore_GamosAnalysis
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
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosAnalysis
	 HEADERS
		GmTrajPointSeco.hh
		GmVEventClassifier.hh
		GmTrajStepProcess.hh
		GmTrajStepPosMom.hh
		GmTrajPointProcess.hh
		GmAnalysisVerbosity.hh
		GmVHistoBuilder.hh
		GmTrajectorySteps.hh
		GmTrajPointPosMom.hh
		GmVTrajStep.hh
		GmCheckOriginalGamma.hh
		GmTrajPoint.hh
		GmTrajStep.hh
		GmEventClassifierByGammaInteraction.hh
		GmTrajStepEner.hh
		GmVHistoMgr.hh
		GmTrajectoryPosMom.hh
		GmVTrajPoint.hh
		GmTrajectory.hh
		GmVTrajectory.hh
	 SOURCES
		GmVTrajectory.cc
		plugin.cc
		GmTrajPointPosMom.cc
		GmCheckOriginalGamma.cc
		GmTrajPointSeco.cc
		GmTrajStepProcess.cc
		GmTrajPoint.cc
		GmAnalysisVerbosity.cc
		GmEventClassifierByGammaInteraction.cc
		GmTrajectory.cc
		GmTrajectoryPosMom.cc
		GmTrajStepEner.cc
		GmTrajectorySteps.cc
		GmVHistoBuilder.cc
		GmTrajPointProcess.cc
		GmTrajStep.cc
		GmTrajStepPosMom.cc
	 
	 GRANULAR_DEPENDENCIES
	 GLOBAL_DEPENDENCIES
		${Geant4_libs} 
		${ROOT_LIBRARIES} 
		${SEAL_LIBRARIES} 
	GamosCore_GamosUtils
	GamosCore_GamosBase_Base
	GamosCore_GamosUserActionMgr

	LINK_LIBRARIES
)
