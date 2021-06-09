#include "Reflex/PluginService.h"

#include "GmSDSimple.hh"
#include "GmSDWithFilter.hh"
#include "GmSDSimpleExactPos.hh"
#include "GmSDVirtSegmentedSphereRThetaPhi.hh"
#include "GmSDVirtSegmentedSphereThetaPhi.hh"
#include "GmSDVirtSegmBox.hh"
#include "GmSDOpticalPhoton.hh"
#include "GmSDSeparateByTime.hh"

PLUGINSVC_FACTORY(GmSDSimple,G4VSensitiveDetector*(G4String))
PLUGINSVC_FACTORY(GmSDSimpleExactPos,G4VSensitiveDetector*(G4String))
PLUGINSVC_FACTORY(GmSDVirtSegmentedSphereRThetaPhi,G4VSensitiveDetector*(G4String))
PLUGINSVC_FACTORY(GmSDVirtSegmentedSphereThetaPhi,G4VSensitiveDetector*(G4String))
PLUGINSVC_FACTORY(GmSDVirtSegmBox,G4VSensitiveDetector*(G4String))
PLUGINSVC_FACTORY(GmSDOpticalPhoton,G4VSensitiveDetector*(G4String))
PLUGINSVC_FACTORY(GmSDSeparateByTime,G4VSensitiveDetector*(G4String))
PLUGINSVC_FACTORY(GmSDWithFilter,G4VSensitiveDetector*(G4String))

#include "GmRecHitBuilderByDistance.hh"
#include "GmRecHitBuilderByBlock.hh"
#include "GmRecHitBuilder1to1.hh"

PLUGINSVC_FACTORY(GmRecHitBuilderByDistance,GmVRecHitBuilder*())
PLUGINSVC_FACTORY(GmRecHitBuilderByBlock,GmVRecHitBuilder*())
PLUGINSVC_FACTORY(GmRecHitBuilder1to1,GmVRecHitBuilder*())

#include "GmHitsHistosUA.hh"
#include "GmRecHitsHistosUA.hh"
#include "GmHitsWriteUA.hh"
#include "GmHitsReadUA.hh"
#include "GmRecHitsWriteUA.hh"
#include "GmHistosGammaAtSD.hh"

PLUGINSVC_FACTORY(GmHitsHistosUA,GmUserAction*())
PLUGINSVC_FACTORY(GmRecHitsHistosUA,GmUserAction*())
PLUGINSVC_FACTORY(GmHitsWriteUA,GmUserAction*())
PLUGINSVC_FACTORY(GmHitsReadUA,GmUserAction*())
PLUGINSVC_FACTORY(GmRecHitsWriteUA,GmUserAction*())
PLUGINSVC_FACTORY(GmHistosGammaAtSD,GmUserAction*())

#include "GmSDVerbosity.hh"
PLUGINSVC_FACTORY(GmSDVerbosity,GmVVerbosity*())
