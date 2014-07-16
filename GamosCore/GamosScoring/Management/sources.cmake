#------------------------------------------------------------------------------
# 
# Module : GamosCore
# Package: GamosScoring_Management
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
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosScoring_Management

   HEADERS
       GmScoringRun.hh
       GmScoringVerbosity.hh
       GmVPrimitiveScorer.hh
       GmVPSPrinter.hh
       GmScoringMessenger.hh
       GmScoringMgr.hh
       GmVChangeWorldAndSource.hh
       GmPSPrinterMgr.hh
       GmVPrimitiveScorerVector.hh
       GmScoringUA.hh
       GmEnergySplitter.hh

   SOURCES
       GmScoringVerbosity.cc
       plugin.cc
       GmEnergySplitter.cc
       GmScoringMgr.cc
       GmScoringMessenger.cc
       GmVPrimitiveScorerVector.cc
       GmPSPrinterMgr.cc
       GmScoringUA.cc
       GmVChangeWorldAndSource.cc
       GmVPrimitiveScorer.cc
       GmScoringRun.cc
       GmVPSPrinter.cc
 
   GRANULAR_DEPENDENCIES

   GLOBAL_DEPENDENCIES
    ${Geant4_LIBRARIES}
    ${Root_LIBRARIES}
  GamosCore_GamosBase_Classifiers
  GamosCore_GamosGenerator
  GamosCore_GamosUtils
  GamosCore_GamosReadDICOM
  GamosCore_GamosUserActionMgr
  GamosCore_GamosBase_Base
  GamosCore_GamosGeometry
  GamosCore_GamosData_Management

   LINK_LIBRARIES
)

# List any source specific properties here
