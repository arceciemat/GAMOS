#ifdef ROOT5
#include "Reflex/PluginService.h"

#include "GmScoringUA.hh"
PLUGINSVC_FACTORY(GmScoringUA,GmUserAction*())

#include "GmScoringVerbosity.hh"
PLUGINSVC_FACTORY(GmScoringVerbosity,GmVVerbosity*())

#include "GmCompoundScorer.hh"
PLUGINSVC_FACTORY(GmCompoundScorer,GmVPrimitiveScorer*(G4String))

#else

#include "GmPrimitiveScorerFactory.hh"
#include "GamosCore/GamosBase/Base/include/GmVerbosityFactory.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserActionFactory.hh"

DEFINE_SEAL_MODULE ();

#include "GmScoringUA.hh"
DEFINE_GAMOS_USER_ACTION(GmScoringUA);

#include "GmCompoundScorer.hh"
DEFINE_GAMOS_SCORER(GmCompoundScorer);

#include "GamosCore/GamosScoring/Management/include/GmScoringVerbosity.hh"
DEFINE_SEAL_PLUGIN(GmVerbosityFactory, GmScoringVerbosity, "GmScoringVerbosity");

#endif
