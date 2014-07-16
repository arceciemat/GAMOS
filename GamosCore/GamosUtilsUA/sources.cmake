#------------------------------------------------------------------------------
# 
# Module : GamosCore
# Package: GamosUtilsUA
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
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosUtilsUA

   HEADERS
       GmTimeStudyUA.hh
       GmChangeWeightUA.hh
       GmCopyWeightToRDSecondaryUA.hh
       GmTimeStudyMgr.hh
       GmSaveHistosAfterNEvents.hh
       GmTrackingVerboseUA.hh
       GmKillAllUA.hh
       GmSaveStepUA.hh
       GmCountTracksAndStepsUA.hh
       GmGamosVerboseByEventUA.hh
       GmMaterialBudgetUA.hh
       GmKillAtSteppingActionUA.hh
       GmCopyWeightToSecondaryUA.hh
       GmCountProcessesUA.hh
       GmShowerShapeUA.hh
       GmStopRunAfterTimeUA.hh
       GmUtilsUAVerbosity.hh
       GmCountTracksUA.hh
       GmKillAtStackingActionUA.hh
       GmGenerHistosUA.hh
       GmSSData.hh
       GmVisStoreTrajUA.hh

   SOURCES
       plugin.cc
       GmGenerHistosUA.cc
       GmCountProcessesUA.cc
       GmCountTracksAndStepsUA.cc
       GmCopyWeightToRDSecondaryUA.cc
       GmTrackingVerboseUA.cc
       GmCopyWeightToSecondaryUA.cc
       GmGamosVerboseByEventUA.cc
       GmKillAtSteppingActionUA.cc
       GmShowerShapeUA.cc
       GmChangeWeightUA.cc
       GmSSData.cc
       GmKillAtStackingActionUA.cc
       GmTimeStudyUA.cc
       GmCountTracksUA.cc
       GmMaterialBudgetUA.cc
       GmSaveHistosAfterNEvents.cc
       GmTimeStudyMgr.cc
       GmVisStoreTrajUA.cc
       GmUtilsUAVerbosity.cc
       GmStopRunAfterTimeUA.cc
       GmKillAllUA.cc
 
   GRANULAR_DEPENDENCIES

   GLOBAL_DEPENDENCIES
    ${Geant4_LIBRARIES}
    ${Root_LIBRARIES}
  GamosCore_GamosBase_Base
  GamosCore_GamosUtils
  GamosCore_GamosAnalysis
  GamosCore_GamosUserActionMgr

   LINK_LIBRARIES
)

# List any source specific properties here
