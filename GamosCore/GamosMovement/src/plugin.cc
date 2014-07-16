#include "Reflex/PluginService.h"

#include "GamosCore/GamosMovement/include/GmMovementEventAction.hh"

PLUGINSVC_FACTORY(GmMovementEventAction,GmUserAction*())

#include "GmMovementVerbosity.hh"
PLUGINSVC_FACTORY(GmMovementVerbosity,GmVVerbosity*())
