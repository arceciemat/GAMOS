#------------------------------------------------------------------------------
# 
# Module : GamosCore
# Package: GamosUtils
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
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosUtils

   HEADERS
       GmCylindricalSurface.hh
       GmCone.hh
       GmPlane.hh
       GmFileIn.hh
       GmG4Utils.hh
       GmMovementUtils.hh
       GmNumberOfEvent.hh
       GmLine.hh
       .hh
       GmGenUtils.hh
       GmVerbosity.hh

   SOURCES
       GmFileIn.cc
       GmCylindricalSurface.cc
       GmG4Utils.cc
       GmGenUtils.cc
       GmVerbosity.cc
       GmMovementUtils.cc
       GmPlane.cc
       GmNumberOfEvent.cc
       GmCone.cc
       GmLine.cc
 
   GRANULAR_DEPENDENCIES

   GLOBAL_DEPENDENCIES
    ${Geant4_LIBRARIES}
    ${Root_LIBRARIES}

   LINK_LIBRARIES
)

# List any source specific properties here
