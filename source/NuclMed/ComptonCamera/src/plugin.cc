#include "CCEventClassifierUA.hh"

#include "CCVerbosity.hh"

#ifdef ROOT5

#include "Reflex/PluginService.h"

PLUGINSVC_FACTORY(CCEventClassifierUA,GmUserAction*())

PLUGINSVC_FACTORY(CCVerbosity,GmVVerbosity*())

#else

#include "GamosCore/GamosBase/Base/include/GmVerbosityFactory.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserActionFactory.hh"

DEFINE_SEAL_MODULE();

DEFINE_GAMOS_USER_ACTION(CCEventClassifierUA);

DEFINE_GAMOS_VERBOSITY(CCVerbosity);

#endif

