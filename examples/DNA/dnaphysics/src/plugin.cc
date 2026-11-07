#include "DNAPPhysicsList.hh"
#include "GmDataDNAPFlagProcess.hh"

#include "GamosCore/GamosPhysics/PhysicsList/include/GmPhysicsFactory.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserActionFactory.hh"
#include "GamosCore/GamosData/Management/include/GmDataFactory.hh"
#include "PluginManager/ModuleDef.h"

DEFINE_SEAL_MODULE ();

DEFINE_GAMOS_PHYSICS(DNAPPhysicsList);
DEFINE_GAMOS_DATA(GmDataDNAPFlagProcess);

