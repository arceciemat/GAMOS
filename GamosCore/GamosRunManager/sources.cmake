#------------------------------------------------------------------------------
# 
# Module : GamosCore
# Package: GamosRunManager
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
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosRunManager

   HEADERS
       GmDeprecatedCommandsMessenger.hh
       GmRunMessenger.hh
       GmFactoriesMessenger.hh
       GmUIMessenger.hh
       GmRunManager.hh
       GmUIterminal.hh

   SOURCES
       plugin.cc
       GmUIMessenger.cc
       GmRunMessenger.cc
       GmDeprecatedCommandsMessenger.cc
       GmUIterminal.cc
       GmFactoriesMessenger.cc
       GmRunManager.cc
 
   GRANULAR_DEPENDENCIES

   GLOBAL_DEPENDENCIES
    ${Geant4_LIBRARIES}
    ${Root_LIBRARIES}
  GamosCore_GamosPhysics_Cuts
  GamosCore_GamosPhysics_PhysicsList
  GamosCore_GamosPhysics_VarianceReduction
  GamosCore_GamosGeometry
  GamosCore_GamosUtils
  GamosCore_GamosScoring_Management
  GamosCore_GamosUserActionMgr
  GamosCore_GamosBase_Base
  GamosCore_GamosSD
  MagFieldManager

   LINK_LIBRARIES
)

# List any source specific properties here
