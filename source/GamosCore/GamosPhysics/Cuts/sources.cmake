#------------------------------------------------------------------------------
# Module : Cuts
# Package: GamosCore_GamosPhysics_Cuts
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
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosPhysics_Cuts
	 HEADERS
		GmCSTrackStepInfo.hh
		GmProdCutsStudyUA.hh
		GmMinRangeLimitsStudyCreateInfoUA.hh
		GmUserLimitsMessenger.hh
		GmCutsTrackInfo.hh
		GmUserLimitsMgr.hh
		GmCutsStudyMgr.hh
		GmProdCutsForAllProcessesUA.hh
		GmUserSpecialCuts.hh
		GmCutsEnergy2Range.hh
		GmStepLimiter.hh
		GmRangeFilter.hh
		GmCSTrackInfo.hh
		GmInitialRangeFilter.hh
		GmRangeRejectionUA.hh
		GmCutsEnergy2RangeUA.hh
		GmMinRangeLimitsStudyUA.hh
	 SOURCES
		GmCutsTrackInfo.cc
		GmProdCutsStudyUA.cc
		plugin.cc
		GmInitialRangeFilter.cc
		GmCSTrackInfo.cc
		GmProdCutsForAllProcessesUA.cc
		GmCutsEnergy2Range.cc
		GmRangeFilter.cc
		GmCSTrackStepInfo.cc
		GmCutsStudyMgr.cc
		GmUserLimitsMessenger.cc
		GmUserSpecialCuts.cc
		GmMinRangeLimitsStudyUA.cc
		GmUserLimitsMgr.cc
		GmMinRangeLimitsStudyCreateInfoUA.cc
		GmStepLimiter.cc
		GmRangeRejectionUA.cc
		GmCutsEnergy2RangeUA.cc
	 
	 GRANULAR_DEPENDENCIES
	 GLOBAL_DEPENDENCIES
		${Geant4_libs} 
		${ROOT_LIBRARIES} 
	GamosCore_GamosUtils
	GamosCore_GamosGeometry
	GamosCore_GamosPhysics_PhysicsList
		${SEAL_LIBRARIES} 

	LINK_LIBRARIES
)
