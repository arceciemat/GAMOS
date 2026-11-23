#ifdef ROOT5
#include "Reflex/PluginService.h"

#include "GmDataVerbosity.hh"
PLUGINSVC_FACTORY(GmDataVerbosity,GmVVerbosity*())
#else
#include "SEAL_Foundation/PluginManager/PluginManager/ModuleDef.h"
DEFINE_SEAL_MODULE ();

#include "GamosCore/GamosBase/Base/include/GmVerbosityFactory.hh"
#include "GmDataVerbosity.hh"
DEFINE_SEAL_PLUGIN(GmVerbosityFactory, GmDataVerbosity, "GmDataVerbosity");
#endif
