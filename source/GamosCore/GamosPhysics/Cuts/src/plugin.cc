#include "GmCutsEnergy2RangeUA.hh"
#include "GmProdCutsStudyUA.hh"
#include "GmMinRangeLimitsStudyUA.hh"
#include "GmRangeRejectionUA.hh"

#include "GmRangeFilter.hh"
#include "GmInitialRangeFilter.hh"

#ifdef ROOT5

#include "Reflex/PluginService.h"

PLUGINSVC_FACTORY(GmCutsEnergy2RangeUA,GmUserAction*())
PLUGINSVC_FACTORY(GmMinRangeLimitsStudyUA,GmUserAction*())
PLUGINSVC_FACTORY(GmProdCutsStudyUA,GmUserAction*())
PLUGINSVC_FACTORY(GmRangeRejectionUA,GmUserAction*())

PLUGINSVC_FACTORY(GmRangeFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmInitialRangeFilter,GmVFilter*(G4String))

#else
//#include "GmProdCutsForAllProcessesUA.hh"

#include "GamosCore/GamosUserActionMgr/include/GmUserActionFactory.hh"

DEFINE_SEAL_MODULE ();

DEFINE_GAMOS_USER_ACTION(GmCutsEnergy2RangeUA);
// DEFINE_GAMOS_USER_ACTION(GmProdCutsForAllProcessesUA);
DEFINE_GAMOS_USER_ACTION(GmMinRangeLimitsStudyUA);
DEFINE_GAMOS_USER_ACTION(GmProdCutsStudyUA);
DEFINE_GAMOS_USER_ACTION(GmRangeRejectionUA);

#include "GamosCore/GamosBase/Base/include/GmFilterFactory.hh"

DEFINE_GAMOS_FILTER(GmRangeFilter);
DEFINE_GAMOS_FILTER(GmInitialRangeFilter);

#endif
