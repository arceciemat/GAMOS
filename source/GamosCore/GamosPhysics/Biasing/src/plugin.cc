#include "GmBOptrForceCollision.hh"
#include "GmBOptrChangeCrossSection.hh"
#include "GmBOptrBremsSplitting.hh"
#include "GmBOptrDirBremsSplitting.hh"
#include "GmBOptrChangeCrossSectionByDistribution.hh"

#include "GmBOptrEWBS.hh"

#ifdef ROOT5

#include "Reflex/PluginService.h"

PLUGINSVC_FACTORY(GmBOptrForceCollision,GmVBiasingOperator*(G4String))
PLUGINSVC_FACTORY(GmBOptrChangeCrossSection,GmVBiasingOperator*(G4String))
PLUGINSVC_FACTORY(GmBOptrBremsSplitting,GmVBiasingOperator*(G4String))
PLUGINSVC_FACTORY(GmBOptrDirBremsSplitting,GmVBiasingOperator*(G4String))
PLUGINSVC_FACTORY(GmBOptrEWBS,GmVBiasingOperator*(G4String))
PLUGINSVC_FACTORY(GmBOptrChangeCrossSectionByDistribution,GmVBiasingOperator*(G4String))

#include "GmBiasingVerbosity.hh"
PLUGINSVC_FACTORY(GmBiasingVerbosity, GmVVerbosity*())

#else

#include "SEAL_Foundation/PluginManager/PluginManager/ModuleDef.h"
#include "GmBiasingOperatorFactory.hh"
#include "GamosCore/GamosBase/Base/include/GmVerbosityFactory.hh"
#include "GmBiasingVerbosity.hh"


DEFINE_SEAL_MODULE ();

DEFINE_GAMOS_BIASINGOPERATOR(GmBOptrForceCollision);
DEFINE_GAMOS_BIASINGOPERATOR(GmBOptrChangeCrossSection);
DEFINE_GAMOS_BIASINGOPERATOR(GmBOptrBremsSplitting);
DEFINE_GAMOS_BIASINGOPERATOR(GmBOptrDirBremsSplitting);
DEFINE_GAMOS_BIASINGOPERATOR(GmBOptrEWBS);
DEFINE_GAMOS_BIASINGOPERATOR(GmBOptrChangeCrossSectionByDistribution);

#include "GmBiasingVerbosity.hh"
DEFINE_SEAL_PLUGIN(GmVerbosityFactory, GmBiasingVerbosity, "GmBiasingVerbosity")
;

#endif
