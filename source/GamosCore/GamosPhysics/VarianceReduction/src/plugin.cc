#include "Reflex/PluginService.h"

#include "GmBSScanWeightsUA.hh"

#include "GmBSControlHistosUA.hh"
#include "GmPSEMPhysics.hh"

PLUGINSVC_FACTORY(GmBSScanWeightsUA,GmUserAction*())
PLUGINSVC_FACTORY(GmBSControlHistosUA,GmUserAction*())

PLUGINSVC_FACTORY(GmPSEMPhysics,G4VUserPhysicsList*())
