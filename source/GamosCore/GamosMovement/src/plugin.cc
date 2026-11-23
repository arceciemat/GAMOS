#include "GamosCore/GamosMovement/include/GmMovementEventAction.hh"

#ifdef ROOT5
#include "Reflex/PluginService.h"

PLUGINSVC_FACTORY(GmMovementEventAction,GmUserAction*())

#include "GmMovementVerbosity.hh"
PLUGINSVC_FACTORY(GmMovementVerbosity,GmVVerbosity*())

#else

#include "GamosCore/GamosUserActionMgr/include/GmUserActionFactory.hh"
#include "SEAL_Foundation/PluginManager/PluginManager/ModuleDef.h"

DEFINE_SEAL_MODULE ();
DEFINE_GAMOS_USER_ACTION(GmMovementEventAction);

#include "GamosCore/GamosBase/Base/include/GmVerbosityFactory.hh"
#include "GmMovementVerbosity.hh"

DEFINE_SEAL_PLUGIN(GmVerbosityFactory, GmMovementVerbosity, "GmMovementVerbosity");

#endif
