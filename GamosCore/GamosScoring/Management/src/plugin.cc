#include "Reflex/PluginService.h"

#include "GmScoringUA.hh"
PLUGINSVC_FACTORY(GmScoringUA,GmUserAction*())

#include "GmScoringVerbosity.hh"
PLUGINSVC_FACTORY(GmScoringVerbosity,GmVVerbosity*())
