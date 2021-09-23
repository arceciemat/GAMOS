#include "Exer4UserActionA.hh"
#include "Exer4UserActionB.hh"

#ifdef ROOT5
#include "Reflex/PluginService.h"

PLUGINSVC_FACTORY(Exer4UserActionA,GmUserAction*())
PLUGINSVC_FACTORY(Exer4UserActionB,GmUserAction*())

#else 

#include "PluginManager/ModuleDef.h"
#include "GamosCore/GamosUserActionMgr/include/GmUserActionFactory.hh"

DEFINE_SEAL_MODULE ();

DEFINE_GAMOS_USER_ACTION(Exer4UserActionA);
DEFINE_GAMOS_USER_ACTION(Exer4UserActionB);

#endif
