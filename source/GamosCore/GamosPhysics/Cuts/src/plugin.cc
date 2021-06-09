#include "Reflex/PluginService.h"

#include "GmCutsEnergy2RangeUA.hh"
#include "GmProdCutsStudyUA.hh"
#include "GmMinRangeLimitsStudyUA.hh"
#include "GmRangeRejectionUA.hh"

PLUGINSVC_FACTORY(GmCutsEnergy2RangeUA,GmUserAction*())
PLUGINSVC_FACTORY(GmMinRangeLimitsStudyUA,GmUserAction*())
PLUGINSVC_FACTORY(GmProdCutsStudyUA,GmUserAction*())
PLUGINSVC_FACTORY(GmRangeRejectionUA,GmUserAction*())

#include "GmRangeFilter.hh"
#include "GmInitialRangeFilter.hh"
PLUGINSVC_FACTORY(GmRangeFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmInitialRangeFilter,GmVFilter*(G4String))
