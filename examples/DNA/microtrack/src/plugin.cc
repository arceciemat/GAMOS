#include "MUTTrackerSD.hh"

#ifdef ROOT5
#include "Reflex/PluginService.h"

PLUGINSVC_FACTORY(MUTTrackerSD,G4VSensitiveDetector*(G4String))

#else

#include "SEAL_Foundation/PluginManager/PluginManager/ModuleDef.h"
#include "GamosCore/GamosSD/include/GmSensDetFactory.hh"

DEFINE_SEAL_MODULE ();

DEFINE_GAMOS_SENSDET(MUTTrackerSD);

#endif
