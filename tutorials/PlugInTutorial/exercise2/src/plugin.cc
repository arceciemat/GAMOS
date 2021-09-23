#include "Exer2GenerDistPosition.hh"

#ifdef ROOT5
#include "Reflex/PluginService.h"

PLUGINSVC_FACTORY(Exer2GenerDistPosition,GmVGenerDistPosition*())

#else 

#include "PluginManager/ModuleDef.h"
DEFINE_SEAL_MODULE ();
#include "GamosCore/GamosGenerator/include/GmGeneratorDistributionFactories.hh"
DEFINE_GAMOS_GENER_DIST_POSITION(Exer2GenerDistPosition);

#endif
