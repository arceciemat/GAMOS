#ifdef ROOT5
#include "Reflex/PluginService.h"
#include "GmBaseVerbosity.hh"

PLUGINSVC_FACTORY(GmBaseVerbosity, GmVVerbosity*())

#else
#include "GmVerbosityFactory.hh"
#include "GmBaseVerbosity.hh"

DEFINE_SEAL_MODULE();

DEFINE_SEAL_PLUGIN(GmVerbosityFactory, GmBaseVerbosity, "GmBaseVerbosity");

#endif
