#------------------------------------------------------------------------------
# 
# Module : GamosCore
# Package: GamosPhysics_Cuts
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
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosPhysics_Cuts

   HEADERS
       GmUserSpecialCuts.hh
       GmCSTrackInfo.hh
       GmCutsEnergy2RangeUA.hh
       GmMinRangeLimitsStudyCreateInfoUA.hh
       GmCutsTrackInfo.hh
       GmMinRangeLimitsStudyUA.hh
       GmProdCutsStudyUA.hh
       GmRangeRejectionUA.hh
       GmUserLimitsMessenger.hh
       GmStepLimiter.hh
       GmUserLimitsMgr.hh
       GmCutsStudyMgr.hh
       GmInitialRangeFilter.hh
       GmCSTrackStepInfo.hh
       GmProdCutsForAllProcessesUA.hh
       GmRangeFilter.hh
       GmCutsEnergy2Range.hh

   SOURCES
       plugin.cc
       GmProdCutsForAllProcessesUA.cc
       GmUserLimitsMgr.cc
       GmRangeFilter.cc
       GmStepLimiter.cc
       GmInitialRangeFilter.cc
       GmMinRangeLimitsStudyCreateInfoUA.cc
       GmCutsEnergy2Range.cc
       GmCSTrackInfo.cc
       GmCutsStudyMgr.cc
       GmMinRangeLimitsStudyUA.cc
       GmRangeRejectionUA.cc
       GmProdCutsStudyUA.cc
       GmCSTrackStepInfo.cc
       GmUserSpecialCuts.cc
       GmCutsTrackInfo.cc
       GmCutsEnergy2RangeUA.cc
       GmUserLimitsMessenger.cc
 
   GRANULAR_DEPENDENCIES

   GLOBAL_DEPENDENCIES
    ${Geant4_LIBRARIES}
    ${Root_LIBRARIES}
  GamosCore_GamosUtils
  GamosCore_GamosBase_Base
  GamosCore_GamosPhysics_PhysicsList
  GamosCore_GamosUserActionMgr
  GamosCore_GamosGeometry

   LINK_LIBRARIES
)

# List any source specific properties here
