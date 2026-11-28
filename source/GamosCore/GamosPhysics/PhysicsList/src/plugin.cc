#include "GmEMPhysics.hh"
#include "GmEMExtendedPhysics.hh"
#include "HadrontherapyPhysics.hh"
#include "GmEMStandardPhysics.hh"
#include "GmRadiotherapy.hh"
#include "GmProtontherapy.hh"

#ifdef ROOT5
#include "Reflex/PluginService.h"

PLUGINSVC_FACTORY(GmEMPhysics,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(GmEMExtendedPhysics,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(HadrontherapyPhysics,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(GmEMStandardPhysics,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(GmRadiotherapy,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(GmProtontherapy,G4VUserPhysicsList*())

#include "GmPhysicsVerbosity.hh"
PLUGINSVC_FACTORY(GmPhysicsVerbosity, GmVVerbosity*())

#else

#include "SEAL_Foundation/PluginManager/PluginManager/ModuleDef.h"
#include "GmPhysicsFactory.hh"
#include "GamosCore/GamosBase/Base/include/GmVerbosityFactory.hh"
#include "GmPhysicsVerbosity.hh"


DEFINE_SEAL_MODULE ();
DEFINE_GAMOS_PHYSICS (GmEMPhysics);
DEFINE_GAMOS_PHYSICS (GmEMExtendedPhysics);
DEFINE_GAMOS_PHYSICS (HadrontherapyPhysics);
DEFINE_GAMOS_PHYSICS (GmEMStandardPhysics);
DEFINE_GAMOS_PHYSICS (GmRadiotherapy);
DEFINE_GAMOS_PHYSICS (GmProtontherapy);

DEFINE_SEAL_PLUGIN(GmVerbosityFactory, GmPhysicsVerbosity, "GmPhysicsVerbosity");

#endif
