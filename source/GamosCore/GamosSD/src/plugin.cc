#include "GmSDSimple.hh"
#include "GmSDWithFilter.hh"
#include "GmSDSimpleExactPos.hh"
#include "GmSDSimpleSmearDOI.hh"
#include "GmSDVirtSegmentedSphereRThetaPhi.hh"
#include "GmSDVirtSegmentedSphereThetaPhi.hh"
#include "GmSDVirtSegmBox.hh"
#include "GmSDOpticalPhoton.hh"
#include "GmSDSeparateByTime.hh"
#include "GmSDFirstInter.hh"

#include "GmRecHitBuilderByDistance.hh"
#include "GmRecHitBuilderByBlock.hh"
#include "GmRecHitBuilderByBlockFirstInter.hh"
#include "GmRecHitBuilder1to1.hh"
#include "GmRecHitBuilderByEMax.hh"
#include "GmRecHitBuilderSmearDOIByEMax.hh"
#include "GmRecHitBuilder1AroundInBlock.hh"

#include "GmHitsHistosUA.hh"
#include "GmRecHitsHistosUA.hh"
#include "GmHitsWriteUA.hh"
#include "GmHitsReadUA.hh"
#include "GmRecHitsWriteUA.hh"
#include "GmHistosGammaAtSD.hh"

#ifdef ROOT5
#include "Reflex/PluginService.h"

PLUGINSVC_FACTORY(GmSDSimple,G4VSensitiveDetector*(G4String))
PLUGINSVC_FACTORY(GmSDSimpleExactPos,G4VSensitiveDetector*(G4String))
PLUGINSVC_FACTORY(GmSDSimpleSmearDOI,G4VSensitiveDetector*(G4String))
PLUGINSVC_FACTORY(GmSDVirtSegmentedSphereRThetaPhi,G4VSensitiveDetector*(G4String))
PLUGINSVC_FACTORY(GmSDVirtSegmentedSphereThetaPhi,G4VSensitiveDetector*(G4String))
PLUGINSVC_FACTORY(GmSDVirtSegmBox,G4VSensitiveDetector*(G4String))
PLUGINSVC_FACTORY(GmSDOpticalPhoton,G4VSensitiveDetector*(G4String))
PLUGINSVC_FACTORY(GmSDSeparateByTime,G4VSensitiveDetector*(G4String))
PLUGINSVC_FACTORY(GmSDWithFilter,G4VSensitiveDetector*(G4String))
PLUGINSVC_FACTORY(GmSDFirstInter,G4VSensitiveDetector*(G4String))

PLUGINSVC_FACTORY(GmRecHitBuilderByDistance,GmVRecHitBuilder*())
PLUGINSVC_FACTORY(GmRecHitBuilderByBlock,GmVRecHitBuilder*())
PLUGINSVC_FACTORY(GmRecHitBuilderByBlockFirstInter,GmVRecHitBuilder*())
PLUGINSVC_FACTORY(GmRecHitBuilder1to1,GmVRecHitBuilder*())
PLUGINSVC_FACTORY(GmRecHitBuilderByEMax,GmVRecHitBuilder*())
PLUGINSVC_FACTORY(GmRecHitBuilderSmearDOIByEMax,GmVRecHitBuilder*())
PLUGINSVC_FACTORY(GmRecHitBuilder1AroundInBlock,GmVRecHitBuilder*())

PLUGINSVC_FACTORY(GmHitsHistosUA,GmUserAction*())
PLUGINSVC_FACTORY(GmRecHitsHistosUA,GmUserAction*())
PLUGINSVC_FACTORY(GmHitsWriteUA,GmUserAction*())
PLUGINSVC_FACTORY(GmHitsReadUA,GmUserAction*())
PLUGINSVC_FACTORY(GmRecHitsWriteUA,GmUserAction*())
PLUGINSVC_FACTORY(GmHistosGammaAtSD,GmUserAction*())

#include "GmSDVerbosity.hh"
PLUGINSVC_FACTORY(GmSDVerbosity,GmVVerbosity*())

#else

#include "SEAL_Foundation/PluginManager/PluginManager/ModuleDef.h"
#include "GmSensDetFactory.hh"
#include "GmRecHitBuilderFactory.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserActionFactory.hh"

DEFINE_SEAL_MODULE ();

DEFINE_GAMOS_SENSDET(GmSDSimple);
DEFINE_GAMOS_SENSDET(GmSDSimpleExactPos);
DEFINE_GAMOS_SENSDET(GmSDSimpleSmearDOI);
DEFINE_GAMOS_SENSDET(GmSDVirtSegmentedSphereRThetaPhi);
DEFINE_GAMOS_SENSDET(GmSDVirtSegmentedSphereThetaPhi);
DEFINE_GAMOS_SENSDET(GmSDVirtSegmBox);
DEFINE_GAMOS_SENSDET(GmSDOpticalPhoton);
DEFINE_GAMOS_SENSDET(GmSDSeparateByTime);
DEFINE_GAMOS_SENSDET(GmSDWithFilter);
DEFINE_GAMOS_SENSDET(GmSDFirstInter);

DEFINE_GAMOS_RECHIT_BUILDER(GmRecHitBuilderByDistance);
DEFINE_GAMOS_RECHIT_BUILDER(GmRecHitBuilderByBlock);
DEFINE_GAMOS_RECHIT_BUILDER(GmRecHitBuilderByBlockFirstInter);
DEFINE_GAMOS_RECHIT_BUILDER(GmRecHitBuilder1to1);
DEFINE_GAMOS_RECHIT_BUILDER(GmRecHitBuilderByEMax);
DEFINE_GAMOS_RECHIT_BUILDER(GmRecHitBuilderSmearDOIByEMax);
DEFINE_GAMOS_RECHIT_BUILDER(GmRecHitBuilder1AroundInBlock);

DEFINE_GAMOS_USER_ACTION(GmHitsHistosUA);
DEFINE_GAMOS_USER_ACTION(GmRecHitsHistosUA);
DEFINE_GAMOS_USER_ACTION(GmHitsWriteUA);
DEFINE_GAMOS_USER_ACTION(GmHitsReadUA);
DEFINE_GAMOS_USER_ACTION(GmRecHitsWriteUA);
DEFINE_GAMOS_USER_ACTION(GmHistosGammaAtSD);

#include "GamosCore/GamosBase/Base/include/GmVerbosityFactory.hh"
#include "GmSDVerbosity.hh"
DEFINE_GAMOS_VERBOSITY(GmSDVerbosity);

#endif
