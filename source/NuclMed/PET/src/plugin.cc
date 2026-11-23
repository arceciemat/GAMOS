
#include "PETHistosPositron.hh"
#include "PETEventClassifierUA.hh"
#include "PETEventClassifierTOFUA.hh"
#include "PETEventClassifierCrystalIDUA.hh"
#include "PETVerbosity.hh" 

#ifdef ROOT5

#include "Reflex/PluginService.h"

PLUGINSVC_FACTORY(PETHistosPositron,GmUserAction*())
PLUGINSVC_FACTORY(PETEventClassifierUA,GmUserAction*())
PLUGINSVC_FACTORY(PETEventClassifierTOFUA,GmUserAction*())
PLUGINSVC_FACTORY(PETEventClassifierCrystalIDUA,GmUserAction*())

PLUGINSVC_FACTORY(PETVerbosity,GmVVerbosity*())

#else

#include "GamosCore/GamosBase/Base/include/GmVerbosityFactory.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserActionFactory.hh"

DEFINE_SEAL_MODULE();

DEFINE_GAMOS_USER_ACTION(PETHistosPositron);
DEFINE_GAMOS_USER_ACTION(PETEventClassifierUA);
DEFINE_GAMOS_USER_ACTION(PETEventClassifierTOFUA);
DEFINE_GAMOS_USER_ACTION(PETEventClassifierCrystalIDUA);

DEFINE_GAMOS_VERBOSITY(PETVerbosity);

#endif
