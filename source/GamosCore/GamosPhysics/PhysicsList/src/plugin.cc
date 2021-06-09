#include "Reflex/PluginService.h"

#include "GmEMPhysics.hh"
#include "GmEMExtendedPhysics.hh"
#include "HadrontherapyPhysics.hh"
#include "GmEMStandardPhysics.hh"

PLUGINSVC_FACTORY(GmEMPhysics,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(GmEMExtendedPhysics,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(HadrontherapyPhysics,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(GmEMStandardPhysics,G4VUserPhysicsList*())

#include "GmPhysicsVerbosity.hh"
PLUGINSVC_FACTORY(GmPhysicsVerbosity, GmVVerbosity*())

