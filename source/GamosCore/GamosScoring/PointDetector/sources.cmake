#------------------------------------------------------------------------------
# 
# Module : GamosCore
# Package: GamosScoring_PointDetector
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
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosScoring_PointDetector

   HEADERS
       GmPDSProcessHelper.hh
       GmPDSInvertGeantinoStackUA.hh
       GmPDSNeutronProcess.hh
       GmPDSVProcess.hh
       GmPDSScoringTrackInfo.hh
       GmPDSGammaProcess.hh
       GmPDSCreateAngleTablesUA.hh
       GmPDSScore.hh
       GmPDSInteractionAngleManager.hh
       GmPDSUA.hh
       GmPDSUtils.hh
       GmPDSGeantinoProcess.hh
       GmPDSDetector.hh

   SOURCES
       plugin.cc
       GmPDSProcessHelper.cc
       GmPDSUA.cc
       GmPDSCreateAngleTablesUA.cc
       GmPDSInteractionAngleManager.cc
       GmPDSNeutronProcess.cc
       GmPDSDetector.cc
       GmPDSUtils.cc
       GmPDSInvertGeantinoStackUA.cc
       GmPDSGeantinoProcess.cc
       GmPDSGammaProcess.cc
       GmPDSVProcess.cc
       GmPDSScore.cc
 
   GRANULAR_DEPENDENCIES

   GLOBAL_DEPENDENCIES
    ${Geant4_LIBRARIES}
    ${Root_LIBRARIES}
  GamosCore_GamosAnalysis
  GamosCore_GamosGeometry
  GamosCore_GamosUtils
  GamosCore_GamosScoring_Management
  GamosCore_GamosUserActionMgr
  GamosCore_GamosBase_Base

   LINK_LIBRARIES
)

# List any source specific properties here
