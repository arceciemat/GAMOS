#include "RTPhaseSpaceUA.hh"
#include "RTPhaseSpace_extUA.hh"
#include "RTPhaseSpace_EGSUA.hh"
#include "RTGeneratorPhaseSpace.hh"
#include "RTGeneratorPhaseSpace_EGS.hh"
#include "RTGeneratorPSPenelope.hh"
#include "RTGeneratorPhaseSpaceRTPlan.hh"
#include "RTZRLimitsAutoUA.hh"
#include "RTZRLimitsUA.hh"
#include "RTVerbosity.hh"
#include "RTPSPDoseHistos.hh"
#include "RTReuseAtZPlaneUA.hh"
#include "RTReuseAtZPlaneWithNVoxelUA.hh"

#include "RTProdCutsStudyUA.hh"
#include "RTMinRangeLimitsStudyUA.hh"
#include "RTCutsStudyFilter.hh"
#include "RTClassifierByEILong.hh"
#include "RTExtraInfoProviderOrigin.hh"
#include "RTExtraInfoProviderInteractions.hh"
#include "RTExtraInfoProviderCrossings.hh"
#include "RTExtraInfoProviderZLast.hh"
#include "RTExtraInfoProviderZOrigin.hh"
#include "RTFilterByEILong.hh"

#ifdef ROOT5
#include "Reflex/PluginService.h"

PLUGINSVC_FACTORY(RTPhaseSpaceUA,GmUserAction*())
PLUGINSVC_FACTORY(RTPhaseSpace_extUA,GmUserAction*())
PLUGINSVC_FACTORY(RTPhaseSpace_EGSUA,GmUserAction*())
PLUGINSVC_FACTORY(RTGeneratorPhaseSpace,G4VUserPrimaryGeneratorAction*())
PLUGINSVC_FACTORY(RTGeneratorPhaseSpace_EGS,G4VUserPrimaryGeneratorAction*())
PLUGINSVC_FACTORY(RTGeneratorPSpenelope,G4VUserPrimaryGeneratorAction*())
PLUGINSVC_FACTORY(RTGeneratorPhaseSpaceRTPlan,G4VUserPrimaryGeneratorAction*())
PLUGINSVC_FACTORY(RTVerbosity,GmVVerbosity*())
PLUGINSVC_FACTORY(RTZRLimitsAutoUA,GmUserAction*())
PLUGINSVC_FACTORY(RTZRLimitsUA,GmUserAction*())
PLUGINSVC_FACTORY(RTPSPDoseHistos,GmVPSPrinter*(G4String))
PLUGINSVC_FACTORY(RTReuseAtZPlaneUA,GmUserAction*())
PLUGINSVC_FACTORY(RTReuseAtZPlaneWithNVoxelUA,GmUserAction*())

PLUGINSVC_FACTORY(RTProdCutsStudyUA,GmUserAction*())
PLUGINSVC_FACTORY(RTMinRangeLimitsStudyUA,GmUserAction*())
PLUGINSVC_FACTORY(RTCutsStudyFilter,GmVFilter*(G4String))

PLUGINSVC_FACTORY(RTExtraInfoProviderOrigin,GmUserAction*())
PLUGINSVC_FACTORY(RTExtraInfoProviderInteractions,GmUserAction*())
PLUGINSVC_FACTORY(RTExtraInfoProviderCrossings,GmUserAction*())
PLUGINSVC_FACTORY(RTExtraInfoProviderZLast,GmUserAction*())
PLUGINSVC_FACTORY(RTExtraInfoProviderZOrigin,GmUserAction*())
PLUGINSVC_FACTORY(RTClassifierByEILong,GmVClassifier*(G4String))
PLUGINSVC_FACTORY(RTFilterByEILong,GmVFilter*(G4String))

#else

#include "GamosCore/GamosUserActionMgr/include/GmUserActionFactory.hh"
#include "GamosCore/GamosGenerator/include/GmGeneratorFactory.hh"
#include "GamosCore/GamosBase/Base/include/GmFilterFactory.hh"
#include "GamosCore/GamosBase/Base/include/GmClassifierFactory.hh"
#include "GamosCore/GamosScoring/Management/include/GmPSPrinterFactory.hh"
#include "GamosCore/GamosBase/Base/include/GmVerbosityFactory.hh"

DEFINE_SEAL_MODULE();

DEFINE_GAMOS_USER_ACTION(RTPhaseSpaceUA);
DEFINE_GAMOS_USER_ACTION(RTPhaseSpace_extUA);
DEFINE_GAMOS_USER_ACTION(RTPhaseSpace_EGSUA);
DEFINE_GAMOS_GENERATOR(RTGeneratorPhaseSpace);
DEFINE_GAMOS_GENERATOR(RTGeneratorPhaseSpace_EGS);
DEFINE_GAMOS_GENERATOR(RTGeneratorPSPenelope);
DEFINE_GAMOS_GENERATOR(RTGeneratorPhaseSpaceRTPlan);
DEFINE_GAMOS_VERBOSITY(RTVerbosity);
DEFINE_GAMOS_USER_ACTION(RTZRLimitsAutoUA);
DEFINE_GAMOS_USER_ACTION(RTZRLimitsUA);
DEFINE_GAMOS_SCORER_PRINTER(RTPSPDoseHistos);
DEFINE_GAMOS_USER_ACTION(RTReuseAtZPlaneUA);
DEFINE_GAMOS_USER_ACTION(RTReuseAtZPlaneWithNVoxelUA);

DEFINE_GAMOS_USER_ACTION(RTProdCutsStudyUA);
DEFINE_GAMOS_USER_ACTION(RTMinRangeLimitsStudyUA);
DEFINE_GAMOS_FILTER(RTCutsStudyFilter);

DEFINE_GAMOS_USER_ACTION(RTExtraInfoProviderOrigin);
DEFINE_GAMOS_USER_ACTION(RTExtraInfoProviderInteractions);
DEFINE_GAMOS_USER_ACTION(RTExtraInfoProviderCrossings);
DEFINE_GAMOS_USER_ACTION(RTExtraInfoProviderZLast);
DEFINE_GAMOS_USER_ACTION(RTExtraInfoProviderZOrigin);
DEFINE_GAMOS_CLASSIFIER(RTClassifierByEILong);
DEFINE_GAMOS_FILTER(RTFilterByEILong);

#endif
