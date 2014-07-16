#include "Reflex/PluginService.h"

#include "GmCountTracksUA.hh"
#include "GmTrackingVerboseUA.hh"
#include "GmCountProcessesUA.hh"
#include "GmCountTracksAndStepsUA.hh"
#include "GmGenerHistosUA.hh"
#include "GmKillAllUA.hh"
#include "GmKillAtStackingActionUA.hh"
#include "GmKillAtSteppingActionUA.hh"
//#include "GmPrintEMPhysicsParamsUA.hh"
#include "GmMaterialBudgetUA.hh"
#include "GmTimeStudyUA.hh"
#include "GmShowerShapeUA.hh"
#include "GmGamosVerboseByEventUA.hh"
#include "GmChangeWeightUA.hh"
#include "GmCopyWeightToSecondaryUA.hh"
#include "GmCopyWeightToRDSecondaryUA.hh"
#include "GmStopRunAfterTimeUA.hh"
#include "GmSaveHistosAfterNEvents.hh"

PLUGINSVC_FACTORY(GmCountTracksUA,GmUserAction*())
PLUGINSVC_FACTORY(GmTrackingVerboseUA,GmUserAction*())
PLUGINSVC_FACTORY(GmCountProcessesUA,GmUserAction*())
PLUGINSVC_FACTORY(GmCountTracksAndStepsUA,GmUserAction*())
PLUGINSVC_FACTORY(GmGenerHistosUA,GmUserAction*())
PLUGINSVC_FACTORY(GmKillAllUA,GmUserAction*())
PLUGINSVC_FACTORY(GmKillAtStackingActionUA,GmUserAction*())
PLUGINSVC_FACTORY(GmKillAtSteppingActionUA,GmUserAction*())
//PLUGINSVC_FACTORY(GmPrintEMPhysicsParamsUA,GmUserAction*())
PLUGINSVC_FACTORY(GmMaterialBudgetUA,GmUserAction*())
PLUGINSVC_FACTORY(GmTimeStudyUA,GmUserAction*())
PLUGINSVC_FACTORY(GmShowerShapeUA,GmUserAction*())
PLUGINSVC_FACTORY(GmGamosVerboseByEventUA,GmUserAction*())
PLUGINSVC_FACTORY(GmChangeWeightUA,GmUserAction*())
PLUGINSVC_FACTORY(GmCopyWeightToSecondaryUA,GmUserAction*())
PLUGINSVC_FACTORY(GmCopyWeightToRDSecondaryUA,GmUserAction*())
PLUGINSVC_FACTORY(GmStopRunAfterTimeUA,GmUserAction*())
PLUGINSVC_FACTORY(GmSaveHistosAfterNEvents,GmUserAction*())

#include "GmUtilsUAVerbosity.hh"
PLUGINSVC_FACTORY(GmUtilsUAVerbosity,GmVVerbosity*())
