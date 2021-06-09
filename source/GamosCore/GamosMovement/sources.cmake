#------------------------------------------------------------------------------
# 
# Module : GamosCore
# Package: GamosMovement
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
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosMovement

   HEADERS
       GmMovementRotation.hh
       GmMovementMgr.hh
       GmMovementNEvents.hh
       GmMovementMessenger.hh
       GmMovementFromFile.hh
       GmMovementEventAction.hh
       GmVMovement.hh
       GmMovementVerbosity.hh
       GmMovementTime.hh
       GmMovementDisplacement.hh

   SOURCES
       plugin.cc
       GmMovementVerbosity.cc
       GmVMovement.cc
       GmMovementRotation.cc
       GmMovementFromFile.cc
       GmMovementDisplacement.cc
       GmMovementEventAction.cc
       GmMovementMgr.cc
       GmMovementMessenger.cc
 
   GRANULAR_DEPENDENCIES

   GLOBAL_DEPENDENCIES
    ${Geant4_LIBRARIES}
    ${Root_LIBRARIES}
  GamosCore_GamosUserActionMgr
  GamosCore_GamosBase_Base
  GamosCore_GamosUtils
  GamosCore_GamosGenerator
  GamosCore_GamosGeometry

   LINK_LIBRARIES
)

# List any source specific properties here
