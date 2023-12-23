#include "GmDNAGeometry.hh"

#include "GmDNASteppingAction.hh"
#include "GmDNATimeStepAction.hh"
#include "GmDNAStackingAction.hh"
#include "GmDataDNAInitialVolumeType.hh"
#include "GmEmDNAPhysics.hh"
#include "GmDNAVerbosity.hh"
#include "GmMicroyzSD.hh" 

#include "GamosCore/GamosGenerator/include/GmGeneratorFactory.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryFactory.hh"
#include "GamosCore/GamosPhysics/PhysicsList/include/GmPhysicsFactory.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserActionFactory.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserTimeStepActionFactory.hh"
#include "GamosCore/GamosData/Management/include/GmDataFactory.hh"
#include "GamosCore/GamosBase/Base/include/GmVerbosityFactory.hh"
#include "GamosCore/GamosSD/include/GmSensDetFactory.hh"

#include "SEAL_Foundation/PluginManager/PluginManager/ModuleDef.h"

DEFINE_SEAL_MODULE ();

DEFINE_GAMOS_GEOMETRY(GmDNAGeometry);

DEFINE_GAMOS_PHYSICS(GmEmDNAPhysics);
DEFINE_GAMOS_USER_ACTION(GmDNASteppingAction);
DEFINE_GAMOS_USER_ACTION(GmDNAStackingAction);
DEFINE_GAMOS_USERTIMESTEP_ACTION(GmDNATimeStepAction);
DEFINE_GAMOS_DATA(GmDataDNAInitialVolumeType);
DEFINE_GAMOS_SENSDET(GmMicroyzSD);


DEFINE_SEAL_PLUGIN(GmVerbosityFactory, GmDNAVerbosity, "GmDNAVerbosity");

