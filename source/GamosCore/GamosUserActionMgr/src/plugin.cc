#include "Reflex/PluginService.h"
#include "GamosCore/GamosUserActionMgr/include/GmUAVerbosity.hh"
PLUGINSVC_FACTORY(GmUAVerbosity,GmVVerbosity*())
 
#include "GmFutureFilter.hh" 
PLUGINSVC_FACTORY(GmFutureFilter,GmVFilter*(G4String))
#include "GmFutureWithChildrenFilter.hh" 
PLUGINSVC_FACTORY(GmFutureWithChildrenFilter,GmVFilter*(G4String))
