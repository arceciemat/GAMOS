#include "GmCheckOriginalGamma.hh"
#include "GmAnalysisVerbosity.hh"

#ifdef ROOT5
#include "Reflex/PluginService.h"

PLUGINSVC_FACTORY(GmCheckOriginalGamma, GmUserAction*())

PLUGINSVC_FACTORY(GmAnalysisVerbosity,GmVVerbosity*())

#else
 
#include "GamosCore/GamosBase/Base/include/GmVerbosityFactory.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserActionFactory.hh"
#include "SEAL_Foundation/PluginManager/PluginManager/ModuleDef.h"
#include "GmAnalysisVerbosity.hh"

DEFINE_SEAL_MODULE ();
DEFINE_GAMOS_VERBOSITY(GmAnalysisVerbosity);

DEFINE_GAMOS_USER_ACTION(GmCheckOriginalGamma);

#endif
