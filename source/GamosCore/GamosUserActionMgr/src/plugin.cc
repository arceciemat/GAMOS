#include "GmFutureFilter.hh"
#include "GmFutureWithChildrenFilter.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUAVerbosity.hh"

#ifdef ROOT5
#include "Reflex/PluginService.h"
PLUGINSVC_FACTORY(GmUAVerbosity,GmVVerbosity*())
 
PLUGINSVC_FACTORY(GmFutureFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmFutureWithChildrenFilter,GmVFilter*(G4String))

#else
#include "SEAL_Foundation/PluginManager/PluginManager/ModuleDef.h"
DEFINE_SEAL_MODULE ();

#include "GamosCore/GamosBase/Base/include/GmVerbosityFactory.hh"

DEFINE_SEAL_PLUGIN(GmVerbosityFactory, GmUAVerbosity, "GmUAVerbosity");

#include "GamosCore/GamosBase/Base/include/GmFilterFactory.hh"
DEFINE_GAMOS_FILTER(GmFutureFilter);
DEFINE_GAMOS_FILTER(GmFutureWithChildrenFilter);

#endif
