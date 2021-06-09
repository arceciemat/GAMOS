#include "SPECTEventClassifierUA.hh"
#include "SPECTVerbosity.hh"

#ifdef ROOT5

#include "Reflex/PluginService.h"
PLUGINSVC_FACTORY(SPECTEventClassifierUA,GmUserAction*())

PLUGINSVC_FACTORY(SPECTVerbosity,GmVVerbosity*())

#else

#include "GamosCore/GamosBase/Base/include/GmVerbosityFactory.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserActionFactory.hh"

DEFINE_SEAL_MODULE();

DEFINE_GAMOS_USER_ACTION(SPECTEventClassifierUA);

DEFINE_GAMOS_VERBOSITY(SPECTVerbosity);

#endif
