#------------------------------------------------------------------------------
# 
# Module : GamosCore
# Package: GamosUserActionMgr
#
#------------------------------------------------------------------------------
# List external includes needed.

# List external includes needed.
include(GamosSetup)
include(UseRoot)

#
# Define the GAMOS Module.
#
include(Geant4MacroDefineModule)
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosUserActionMgr

   HEADERS
       GmUserTrackingAction.hh
       GmUserRunActionList.hh
       GmFutureFilter.hh
       GmUserTrackingActionList.hh
       GmUserActionMgr.hh
       GmUserActionMessenger.hh
       GmUserAction.hh
       GmFutureWithChildrenFilter.hh
       GmUserSteppingAction.hh
       GmUserStackingAction.hh
       GmFutureFilterUA.hh
       GmUAVerbosity.hh
       GmUserStackingActionList.hh
       GmUserEventActionList.hh
       GmUserRunAction.hh
       GmUserSteppingActionList.hh
       GmUserEventAction.hh
       GmStepMgr.hh

   SOURCES
       plugin.cc
       GmUAVerbosity.cc
       GmUserStackingAction.cc
       GmFutureWithChildrenFilter.cc
       GmUserEventActionList.cc
       GmUserSteppingAction.cc
       GmStepMgr.cc
       GmUserSteppingActionList.cc
       GmFutureFilterUA.cc
       GmUserTrackingAction.cc
       GmUserActionMessenger.cc
       GmUserStackingActionList.cc
       GmUserRunActionList.cc
       GmUserAction.cc
       GmUserTrackingActionList.cc
       GmFutureFilter.cc
       GmUserEventAction.cc
       GmUserRunAction.cc
       GmUserActionMgr.cc
 
   GRANULAR_DEPENDENCIES

   GLOBAL_DEPENDENCIES
    ${Geant4_LIBRARIES}
    ${Root_LIBRARIES}
  GamosCore_GamosBase_Base
  GamosCore_GamosUtils

   LINK_LIBRARIES
)

# List any source specific properties here
