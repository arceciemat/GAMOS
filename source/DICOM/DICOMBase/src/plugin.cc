#include "DicomVerbosity.hh"

#ifdef ROOT5
#include "Reflex/PluginService.h"

PLUGINSVC_FACTORY(DicomVerbosity,GmVVerbosity*())

#else 

#include "SEAL_Foundation/PluginManager/PluginManager/ModuleDef.h"
DEFINE_SEAL_MODULE ();

#include "GamosCore/GamosBase/Base/include/GmVerbosityFactory.hh"
DEFINE_SEAL_PLUGIN(GmVerbosityFactory, DicomVerbosity, "DicomVerbosity");
 
#endif
