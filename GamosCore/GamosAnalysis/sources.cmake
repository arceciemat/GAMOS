#------------------------------------------------------------------------------
# 
# Module : GamosCore
# Package: GamosAnalysis
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
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosAnalysis

   HEADERS
       GmTrajectory.hh
       GmTrajPointPosMom.hh
       GmTrajPointProcess.hh
       GmEventClassifierByGammaInteraction.hh
       GmVTrajectory.hh
       GmTrajStepProcess.hh
       GmAnalysisVerbosity.hh
       GmVTrajPoint.hh
       GmTrajStep.hh
       GmVHistoBuilder.hh
       GmTrajPointSeco.hh
       GmTrajStepEner.hh
       GmTrajectoryPosMom.hh
       GmTrajPoint.hh
       GmTrajectorySteps.hh
       GmCheckOriginalGamma.hh
       GmVTrajStep.hh
       GmTrajStepPosMom.hh
       GmVHistoMgr.hh
       GmVEventClassifier.hh

   SOURCES
       plugin.cc
       GmTrajectorySteps.cc
       GmTrajectory.cc
       GmTrajPointProcess.cc
       GmTrajStepEner.cc
       GmVTrajectory.cc
       GmTrajStepProcess.cc
       GmTrajStepPosMom.cc
       GmAnalysisVerbosity.cc
       GmTrajPointSeco.cc
       GmCheckOriginalGamma.cc
       GmTrajectoryPosMom.cc
       GmTrajStep.cc
       GmVHistoBuilder.cc
       GmTrajPointPosMom.cc
       GmEventClassifierByGammaInteraction.cc
       GmTrajPoint.cc
 
   GRANULAR_DEPENDENCIES

   GLOBAL_DEPENDENCIES
    ${Geant4_LIBRARIES}
    ${Root_LIBRARIES}
  GamosCore_GamosBase_Base
  GamosCore_GamosUtils
  GamosCore_GamosUserActionMgr

   LINK_LIBRARIES
)

# List any source specific properties here
