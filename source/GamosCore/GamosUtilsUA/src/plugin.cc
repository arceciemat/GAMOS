#include "GmCountTracksUA.hh"
#include "GmTrackingVerboseUA.hh"
#include "GmCountProcessesUA.hh"
#include "GmCountTracksAndStepsUA.hh"
#include "GmGenerHistosUA.hh"
#include "GmKillAllUA.hh"
#include "GmKillAtStackingActionUA.hh"
#include "GmKillAtStackingActionRandomUA.hh"
#include "GmKillAtSteppingActionUA.hh"
#include "GmKillAtTrackingActionUA.hh"
#include "GmKillTooManyStepsUA.hh"
//#include "GmPrintEMPhysicsParamsUA.hh"
#include "GmMaterialBudgetUA.hh"
#include "GmTimeStudyUA.hh"
#include "GmTimeStudyEventUA.hh"
#include "GmShowerShapeUA.hh"
#include "GmGamosVerboseByEventUA.hh"
#include "GmChangeWeightUA.hh"
#include "GmCopyWeightToSecondaryUA.hh"
#include "GmCopyWeightToRDSecondaryUA.hh"
#include "GmStopRunAfterTimeUA.hh"
#include "GmSaveHistosAfterNEvents.hh"
#include "GmPrintEMParamsUA.hh"
#include "GmRandomKillByParticleUA.hh"
#include "GmSaveHistosAtRTCPUA.hh"
#include "GmVisModelUA.hh"
#include "GmStepRandomWeightUA.hh"
#include "GmTrackRandomWeightUA.hh"

#ifdef ROOT5
#include "Reflex/PluginService.h"

PLUGINSVC_FACTORY(GmCountTracksUA,GmUserAction*())
PLUGINSVC_FACTORY(GmTrackingVerboseUA,GmUserAction*())
PLUGINSVC_FACTORY(GmCountProcessesUA,GmUserAction*())
PLUGINSVC_FACTORY(GmCountTracksAndStepsUA,GmUserAction*())
PLUGINSVC_FACTORY(GmGenerHistosUA,GmUserAction*())
PLUGINSVC_FACTORY(GmKillAllUA,GmUserAction*())
PLUGINSVC_FACTORY(GmKillAtStackingActionUA,GmUserAction*())
PLUGINSVC_FACTORY(GmKillAtStackingActionRandomUA,GmUserAction*())
PLUGINSVC_FACTORY(GmKillAtSteppingActionUA,GmUserAction*())
PLUGINSVC_FACTORY(GmKillAtTrackingActionUA,GmUserAction*())
PLUGINSVC_FACTORY(GmKillTooManyStepsUA,GmUserAction*())
//PLUGINSVC_FACTORY(GmPrintEMPhysicsParamsUA,GmUserAction*())
PLUGINSVC_FACTORY(GmMaterialBudgetUA,GmUserAction*())
PLUGINSVC_FACTORY(GmTimeStudyUA,GmUserAction*())
PLUGINSVC_FACTORY(GmTimeStudyEventUA,GmUserAction*())
PLUGINSVC_FACTORY(GmShowerShapeUA,GmUserAction*())
PLUGINSVC_FACTORY(GmGamosVerboseByEventUA,GmUserAction*())
PLUGINSVC_FACTORY(GmChangeWeightUA,GmUserAction*())
PLUGINSVC_FACTORY(GmCopyWeightToSecondaryUA,GmUserAction*())
PLUGINSVC_FACTORY(GmCopyWeightToRDSecondaryUA,GmUserAction*())
PLUGINSVC_FACTORY(GmStopRunAfterTimeUA,GmUserAction*())
PLUGINSVC_FACTORY(GmSaveHistosAfterNEvents,GmUserAction*())
PLUGINSVC_FACTORY(GmPrintEMParamsUA,GmUserAction*())
PLUGINSVC_FACTORY(GmRandomKillByParticleUA,GmUserAction*())
PLUGINSVC_FACTORY(GmSaveHistosAtRTCPUA,GmUserAction*())
PLUGINSVC_FACTORY(GmVisModelUA,GmUserAction*())
PLUGINSVC_FACTORY(GmStepRandomWeightUA,GmUserAction*())
PLUGINSVC_FACTORY(GmTrackRandomWeightUA,GmUserAction*())

#include "GmUtilsUAVerbosity.hh"
PLUGINSVC_FACTORY(GmUtilsUAVerbosity,GmVVerbosity*())

#else
#include "SEAL_Foundation/PluginManager/PluginManager/ModuleDef.h"
#include "GamosCore/GamosUserActionMgr/include/GmUserActionFactory.hh"

DEFINE_SEAL_MODULE ();

DEFINE_GAMOS_USER_ACTION(GmCountTracksUA);
DEFINE_GAMOS_USER_ACTION(GmTrackingVerboseUA);
DEFINE_GAMOS_USER_ACTION(GmCountProcessesUA);
DEFINE_GAMOS_USER_ACTION(GmCountTracksAndStepsUA);
DEFINE_GAMOS_USER_ACTION(GmGenerHistosUA);
DEFINE_GAMOS_USER_ACTION(GmKillAllUA);
DEFINE_GAMOS_USER_ACTION(GmKillAtStackingActionUA);
DEFINE_GAMOS_USER_ACTION(GmKillAtStackingActionRandomUA);
DEFINE_GAMOS_USER_ACTION(GmKillAtSteppingActionUA);
DEFINE_GAMOS_USER_ACTION(GmKillAtTrackingActionUA);
DEFINE_GAMOS_USER_ACTION(GmKillTooManyStepsUA);
//DEFINE_GAMOS_USER_ACTION(GmPrintEMPhysicsParamsUA);
DEFINE_GAMOS_USER_ACTION(GmMaterialBudgetUA);
DEFINE_GAMOS_USER_ACTION(GmTimeStudyUA);
DEFINE_GAMOS_USER_ACTION(GmTimeStudyEventUA);
DEFINE_GAMOS_USER_ACTION(GmShowerShapeUA);
DEFINE_GAMOS_USER_ACTION(GmGamosVerboseByEventUA);
DEFINE_GAMOS_USER_ACTION(GmChangeWeightUA);
DEFINE_GAMOS_USER_ACTION(GmCopyWeightToSecondaryUA);
DEFINE_GAMOS_USER_ACTION(GmCopyWeightToRDSecondaryUA);
DEFINE_GAMOS_USER_ACTION(GmStopRunAfterTimeUA);
DEFINE_GAMOS_USER_ACTION(GmSaveHistosAfterNEvents);
DEFINE_GAMOS_USER_ACTION(GmPrintEMParamsUA);
DEFINE_GAMOS_USER_ACTION(GmRandomKillByParticleUA);
DEFINE_GAMOS_USER_ACTION(GmSaveHistosAtRTCPUA);
DEFINE_GAMOS_USER_ACTION(GmVisModelUA);
DEFINE_GAMOS_USER_ACTION(GmStepRandomWeightUA);
DEFINE_GAMOS_USER_ACTION(GmTrackRandomWeightUA);

#include "GamosCore/GamosBase/Base/include/GmVerbosityFactory.hh"
#include "GmUtilsUAVerbosity.hh"
DEFINE_GAMOS_VERBOSITY(GmUtilsUAVerbosity);

#include "KillNegativeLengthUA.hh"
DEFINE_GAMOS_USER_ACTION(KillNegativeLengthUA);

#endif
