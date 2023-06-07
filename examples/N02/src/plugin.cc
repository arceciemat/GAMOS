
#include "ExN02DetectorConstruction.hh"
#include "ExN02PrimaryGeneratorAction.hh"
#include "ExN02PhysicsList.hh"
#include "ExN02RunAction.hh"
#include "ExN02EventAction.hh"
#include "ExN02SteppingAction.hh"
#include "ExN02TrackerSD.hh"

#ifdef ROOT5

#include "Reflex/PluginService.h"

PLUGINSVC_FACTORY(ExN02DetectorConstruction,G4VUserDetectorConstruction*())
PLUGINSVC_FACTORY(ExN02PrimaryGeneratorAction,G4VUserPrimaryGeneratorAction*())
PLUGINSVC_FACTORY(ExN02PhysicsList,G4VUserPhysicsList*())
PLUGINSVC_FACTORY(ExN02RunAction,GmUserAction*())
PLUGINSVC_FACTORY(ExN02EventAction,GmUserAction*())
PLUGINSVC_FACTORY(ExN02SteppingAction,GmUserAction*())
PLUGINSVC_FACTORY(ExN02TrackerSD,G4VSensitiveDetector*(G4String))
 
#else 

#include "GamosCore/GamosGenerator/include/GmGeneratorFactory.hh"
#include "GamosCore/GamosGeometry/include/GmGeometryFactory.hh"
#include "GamosCore/GamosPhysics/PhysicsList/include/GmPhysicsFactory.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserActionFactory.hh"
#include "GamosCore/GamosSD/include/GmSensDetFactory.hh"
#include "PluginManager/ModuleDef.h"

DEFINE_SEAL_MODULE ();

DEFINE_GAMOS_GEOMETRY(ExN02DetectorConstruction);
DEFINE_GAMOS_GENERATOR(ExN02PrimaryGeneratorAction);
DEFINE_GAMOS_PHYSICS(ExN02PhysicsList);
DEFINE_GAMOS_USER_ACTION(ExN02RunAction);
DEFINE_GAMOS_USER_ACTION(ExN02EventAction);
DEFINE_GAMOS_USER_ACTION(ExN02SteppingAction);
DEFINE_GAMOS_SENSDET(ExN02TrackerSD);

#endif
