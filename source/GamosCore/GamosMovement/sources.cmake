# - GamosCore_GamosMovement module build definition

gamos_add_module(GamosCore_GamosMovement
  PUBLIC_HEADERS
    GmMovementDisplacement.hh
    GmMovementEventAction.hh
    GmMovementFromFile.hh
    GmMovementMessenger.hh
    GmMovementMgr.hh
    GmMovementNEvents.hh
    GmMovementRotation.hh
    GmMovementTime.hh
    GmMovementVerbosity.hh
    GmVMovement.hh
  SOURCES
    GmMovementDisplacement.cc
    GmMovementEventAction.cc
    GmMovementFromFile.cc
    GmMovementMessenger.cc
    GmMovementMgr.cc
    GmMovementRotation.cc
    GmMovementVerbosity.cc
    GmVMovement.cc
    plugin.cc
)

gamos_module_link_libraries(GamosCore_GamosMovement
  PUBLIC
    GamosCore_GamosBase_Base
    GamosCore_GamosGenerator
    GamosCore_GamosGeometry
    GamosCore_GamosUserActionMgr
    GamosCore_GamosUtils
    lcg_PluginManager
    lcg_SealBase
)
