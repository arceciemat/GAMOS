
#include "SHNthValueLayerUA.hh"
#include "SHActivationUA.hh"
#include "SHActivationFromFluxUA.hh"
#include "SHGetHadronicChannelXSUA.hh"
#include "SHPrintPHPChannelXSUA.hh"
#include "SHStudyDecayChannelsUA.hh"
#include "SHPlotPHPDBSpectraUA.hh"
#include "SHPrintEMDataUA.hh"
#include "SHPrintPHPChannelThickYieldUA.hh"

#include "GmNoUseG4RadDecayTimeUA.hh"
#include "GmSetG4RadDecayTime0UA.hh"
#include "GmRadDecayGammaAngleCorrelationUA.hh"

#include "ShieldingVerbosity.hh"

#ifdef ROOT5

#include "Reflex/PluginService.h"

PLUGINSVC_FACTORY(SHNthValueLayerUA,GmUserAction*())
PLUGINSVC_FACTORY(SHActivationUA,GmUserAction*())
PLUGINSVC_FACTORY(SHActivationFromFluxUA,GmUserAction*())
PLUGINSVC_FACTORY(SHGetHadronicChannelXSUA,GmUserAction*())
PLUGINSVC_FACTORY(SHPrintPHPChannelXSUA,GmUserAction*())
PLUGINSVC_FACTORY(SHStudyDecayChannelsUA,GmUserAction*())
PLUGINSVC_FACTORY(SHPlotPHPDBSpectraUA,GmUserAction*())
PLUGINSVC_FACTORY(SHPrintEMDataUA,GmUserAction*())
PLUGINSVC_FACTORY(SHPrintPHPChannelThickYieldUA,GmUserAction*())

PLUGINSVC_FACTORY(GmNoUseG4RadDecayTimeUA,GmUserAction*())
PLUGINSVC_FACTORY(GmSetG4RadDecayTimeUA0,GmUserAction*())
PLUGINSVC_FACTORY(GmRadDecayGammaAngleCorrelationUA,GmUserAction*())

PLUGINSVC_FACTORY(ShieldingVerbosity,GmVVerbosity*())

#else

#include "GamosCore/GamosBase/Base/include/GmVerbosityFactory.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserActionFactory.hh"

DEFINE_SEAL_MODULE();

DEFINE_GAMOS_USER_ACTION(SHNthValueLayerUA);
DEFINE_GAMOS_USER_ACTION(SHActivationUA);
DEFINE_GAMOS_USER_ACTION(SHActivationFromFluxUA);
DEFINE_GAMOS_USER_ACTION(SHGetHadronicChannelXSUA);
DEFINE_GAMOS_USER_ACTION(SHPrintPHPChannelXSUA);
DEFINE_GAMOS_USER_ACTION(SHStudyDecayChannelsUA);
DEFINE_GAMOS_USER_ACTION(SHPlotPHPDBSpectraUA);
DEFINE_GAMOS_USER_ACTION(SHPrintEMDataUA);
DEFINE_GAMOS_USER_ACTION(SHPrintPHPChannelThickYieldUA);

DEFINE_GAMOS_USER_ACTION(GmNoUseG4RadDecayTimeUA);
DEFINE_GAMOS_USER_ACTION(GmSetG4RadDecayTime0UA);
DEFINE_GAMOS_USER_ACTION(GmRadDecayGammaAngleCorrelationUA);

DEFINE_GAMOS_VERBOSITY(ShieldingVerbosity);

#endif
