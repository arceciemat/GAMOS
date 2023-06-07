# - GamosCore_GamosUserActionMgr module build definition

include(UseGamosAtGeant4)
include(UseROOT)

geant4_add_module(GamosCore_GamosUserActionMgr
  PUBLIC_HEADERS
    GmStepMgr.hh
    GmUserAction.hh
    GmUAVerbosity.hh
    GmFutureFilter.hh
    GmFutureFilterUA.hh
    GmUserStackingActionList.hh
    GmUserSteppingActionList.hh
    GmUserRunAction.hh
    GmUserEventActionList.hh
    GmUserActionMgr.hh
    GmUserRunActionList.hh
    GmUserTrackingActionList.hh
    GmUserTrackingAction.hh
    GmUserActionMgr.icc
    GmFutureWithChildrenFilter.hh
    GmUserActionFactory.hh
    GmUserStackingAction.hh
    GmUserSteppingAction.hh
    GmUserEventAction.hh
    GmUserActionMessenger.hh
  SOURCES
    GmUserActionFactory.cc
    GmUserSteppingActionList.cc
    GmUserStackingActionList.cc
    GmFutureFilter.cc
    GmUserEventAction.cc
    GmFutureWithChildrenFilter.cc
    GmStepMgr.cc
    GmUserTrackingActionList.cc
    plugin.cc
    GmFutureFilterUA.cc
    GmUserActionMgr.cc
    GmUserRunAction.cc
    GmUserStackingAction.cc
    GmUserActionMessenger.cc
    GmUserAction.cc
    GmUAVerbosity.cc
    GmUserTrackingAction.cc
    GmUserRunActionList.cc
    GmUserSteppingAction.cc
    GmUserEventActionList.cc
)
# - Add path to generated header
geant4_module_include_directories(GamosCore_GamosUserActionMgr
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/SealBase/include>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/SealPlatform/include>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/PluginManager/include>
 PUBLIC  $<BUILD_INTERFACE:${ROOT_BASE_DIR}/include>)

geant4_module_link_libraries(GamosCore_GamosUserActionMgr
  PUBLIC
    ${Geant4_libs}
    ${ROOT_LIBRARIES}
    ${SEAL_LIBRARIES} 
    GamosCore_GamosUtils
    GamosCore_GamosBase_Base
)
