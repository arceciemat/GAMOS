#include "GmPDSCreateAngleTablesUA.hh"
#include "GmPDSUA.hh"

#ifdef ROOT5
#include "Reflex/PluginService.h"

PLUGINSVC_FACTORY(GmPDSCreateAngleTablesUA,GmUserAction*())
PLUGINSVC_FACTORY(GmPDSUA,GmUserAction*())

#else
#include "GamosCore/GamosUserActionMgr/include/GmUserActionFactory.hh"

DEFINE_SEAL_MODULE ();

#include "GmPDSCreateAngleTablesUA.hh"
#include "GmPDSUA.hh"

DEFINE_GAMOS_USER_ACTION(GmPDSCreateAngleTablesUA);
DEFINE_GAMOS_USER_ACTION(GmPDSUA);

#endif
