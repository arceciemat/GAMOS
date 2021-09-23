#------------------------------------------------------------------------------
# Module : GamosUserActionMgr
# Package: GamosCore_GamosUserActionMgr
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
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosUserActionMgr
	 HEADERS
		GmFutureFilter.hh
		GmUserRunAction.hh
		GmFutureFilterUA.hh
		GmUserActionMgr.hh
		GmUserRunActionList.hh
		GmUserEventAction.hh
		GmStepMgr.hh
		GmUserActionMessenger.hh
		GmUserActionMgr.icc
		GmUserSteppingActionList.hh
		GmUserStackingActionList.hh
		GmUserTrackingActionList.hh
		GmUserAction.hh
		GmUserActionFactory.hh
		GmUserStackingAction.hh
		GmUserEventActionList.hh
		GmUserSteppingAction.hh
		GmFutureWithChildrenFilter.hh
		GmUserTrackingAction.hh
		GmUAVerbosity.hh
	 SOURCES
		GmUserEventActionList.cc
		plugin.cc
		GmUserEventAction.cc
		GmUserRunAction.cc
		GmUserStackingActionList.cc
		GmUserActionMessenger.cc
		GmUAVerbosity.cc
		GmUserActionMgr.cc
		GmUserActionFactory.cc
		GmUserStackingAction.cc
		GmStepMgr.cc
		GmUserSteppingActionList.cc
		GmUserSteppingAction.cc
		GmFutureWithChildrenFilter.cc
		GmUserTrackingAction.cc
		GmUserAction.cc
		GmFutureFilter.cc
		GmFutureFilterUA.cc
		GmUserTrackingActionList.cc
		GmUserRunActionList.cc
	 
	 GRANULAR_DEPENDENCIES
	 GLOBAL_DEPENDENCIES
		${Geant4_libs} 
		${ROOT_LIBRARIES} 
	GamosCore_GamosUtils
	GamosCore_GamosBase_Base

	LINK_LIBRARIES
)
