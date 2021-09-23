#include "Exer6SD.hh"

#ifdef ROOT5
#include "Reflex/PluginService.h"

PLUGINSVC_FACTORY(Exer6SD,G4VSensitiveDetector*(G4String))
#else 

#include "PluginManager/ModuleDef.h"
DEFINE_SEAL_MODULE ();

#include "GamosCore/GamosSD/include/GmSensDetFactory.hh"
DEFINE_GAMOS_SENSDET(Exer6SD);

#endif
