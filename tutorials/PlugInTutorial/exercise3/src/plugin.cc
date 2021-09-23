#include "eScatteringPhysicsList.hh"

#ifdef ROOT5
#include "Reflex/PluginService.h"

PLUGINSVC_FACTORY(eScatteringPhysicsList,G4VUserPhysicsList*())

#else

#include "PluginManager/ModuleDef.h"
#include "GamosCore/GamosPhysics/PhysicsList/include/GmPhysicsFactory.hh"
DEFINE_SEAL_MODULE ();
DEFINE_GAMOS_PHYSICS (eScatteringPhysicsList);

#endif
