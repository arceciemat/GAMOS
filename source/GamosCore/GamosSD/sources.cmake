#------------------------------------------------------------------------------
# 
# Module : GamosCore
# Package: GamosSD
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
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosSD

   HEADERS
       GmDeadTimeDUListByBlock.hh
       GmTrajPointSD.hh
       GmSDVirtSegmentedSphereRThetaPhi.hh
       GmHistosGammaAtSD.hh
       GmSDWithFilter.hh
       GmVRecHitsIOMgr.hh
       GmRecHitBuilder1to1.hh
       GmDeadTimeDUListByCrystal.hh
       GmEDepo.hh
       GmRecHitBuilderByBlock.hh
       GmVRecHitBuilderFromDigits.hh
       GmSDOpticalPhoton.hh
       GmVHitsIOMgr.hh
       GmVRecHitBuilderFromHits.hh
       GmEventTimeExtractorFromHit.hh
       GmVDigitizer.hh
       GmHitsReadUA.hh
       GmRecHit.hh
       GmRecHitPair.hh
       GmEventTimeExtractorFromFirstTrack.hh
       GmSDSeparateByTime.hh
       GmHitsIOtextMgr.hh
       GmRecHitsWriteUA.hh
       GmVSD.hh
       GmSDType.hh
       GmDigit.hh
       GmHitsEventMgr.hh
       GmHitsWriteUA.hh
       GmVVirtSegmSD.hh
       GmSDVirtSegmBox.hh
       GmHitList.hh
       GmSDSimpleExactPos.hh
       GmVRecHitBuilder.hh
       GmDetUnitIDBuilderFromAncestors.hh
       GmHitsHistosUA.hh
       GmSDVirtSegmentedSphere.hh
       GmRecHitsIObinMgr.hh
       GmVSDList.hh
       GmRecHitsIOtextMgr.hh
       GmSDVerbosity.hh
       GmSDMessenger.hh
       GmVEventTimeExtractor.hh
       GmRecHitBuilderByDistance.hh
       GmDetUnitIDBuilder.hh
       GmSDSimple.hh
       GmSDVirtSegmentedSphereXYZ.hh
       GmHit.hh
       GmHitsIObinMgr.hh
       GmSDVirtSegmentedSphereThetaPhi.hh
       GmVDeadTimeDUList.hh
       GmRecHitsHistosUA.hh

   SOURCES
       plugin.cc
       GmRecHitPair.cc
       GmTrajPointSD.cc
       GmHitsIOtextMgr.cc
       GmRecHitBuilder1to1.cc
       GmVRecHitsIOMgr.cc
       GmSDVerbosity.cc
       GmDigit.cc
       GmRecHitsHistosUA.cc
       GmSDVirtSegmentedSphere.cc
       GmSDVirtSegmentedSphereRThetaPhi.cc
       GmVHitsIOMgr.cc
       GmHitsHistosUA.cc
       GmSDWithFilter.cc
       GmSDMessenger.cc
       GmRecHit.cc
       GmDeadTimeDUListByBlock.cc
       GmRecHitsWriteUA.cc
       GmVRecHitBuilder.cc
       GmVRecHitBuilderFromHits.cc
       GmEventTimeExtractorFromFirstTrack.cc
       GmVSD.cc
       GmHitList.cc
       GmSDSimpleExactPos.cc
       GmHitsEventMgr.cc
       GmSDOpticalPhoton.cc
       GmDeadTimeDUListByCrystal.cc
       GmVSDList.cc
       GmHitsReadUA.cc
       GmHitsIObinMgr.cc
       GmEventTimeExtractorFromHit.cc
       GmSDVirtSegmBox.cc
       GmHitsWriteUA.cc
       GmEDepo.cc
       GmHistosGammaAtSD.cc
       GmSDSimple.cc
       GmHit.cc
       GmSDVirtSegmentedSphereThetaPhi.cc
       GmVRecHitBuilderFromDigits.cc
       GmRecHitBuilderByBlock.cc
       GmSDSeparateByTime.cc
       GmRecHitsIObinMgr.cc
       GmRecHitsIOtextMgr.cc
       GmDetUnitIDBuilderFromAncestors.cc
       GmSDType.cc
       GmRecHitBuilderByDistance.cc
       GmVDeadTimeDUList.cc
 
   GRANULAR_DEPENDENCIES

   GLOBAL_DEPENDENCIES
    ${Geant4_LIBRARIES}
    ${Root_LIBRARIES}
  GamosCore_GamosAnalysis
  GamosCore_GamosGeometry
  GamosCore_GamosData_Distributions
  GamosCore_GamosUtils
  GamosCore_GamosUserActionMgr
  GamosCore_GamosBase_Base
  GamosCore_GamosGenerator

   LINK_LIBRARIES
)

# List any source specific properties here
