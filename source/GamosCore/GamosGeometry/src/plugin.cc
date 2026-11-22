#include "GmGeometryFromText.hh"
#include "GmGeomTextDumperUA.hh"

#include "GmGeometryFromText.hh"
#include "GmGeometryUseMaterials.hh"
#include "GmGeomTextDumperUA.hh"

#ifdef ROOT5
#include "Reflex/PluginService.h"
//#include "GamosCore/GamosGeometry/include/GDMLDetectorConstruction.hh"

PLUGINSVC_FACTORY (GmGeometryFromText,G4VUserDetectorConstruction*())
PLUGINSVC_FACTORY(GmGeometryUseMaterials,G4VUserDetectorConstruction*())

PLUGINSVC_FACTORY(GmGeomTextDumperUA,GmUserAction*())

#include "GmGeomVerbosity.hh"
PLUGINSVC_FACTORY(GmGeomVerbosity,GmVVerbosity*())

#else 

#include "SEAL_Foundation/PluginManager/PluginManager/ModuleDef.h"
DEFINE_SEAL_MODULE ();

#include "GmGeometryFactory.hh"
//DEFINE_GAMOS_GEOMETRY (GDMLDetectorConstruction);
DEFINE_GAMOS_GEOMETRY (GmGeometryFromText);
//DEFINE_GAMOS_GEOMETRY (GmGeometryFromTextOld);
DEFINE_GAMOS_GEOMETRY(GmGeometryUseMaterials);

#include "GamosCore/GamosUserActionMgr/include/GmUserActionFactory.hh"
DEFINE_GAMOS_USER_ACTION(GmGeomTextDumperUA);

#include "GamosCore/GamosBase/Base/include/GmVerbosityFactory.hh"
#include "GmGeomVerbosity.hh"
DEFINE_SEAL_PLUGIN(GmVerbosityFactory, GmGeomVerbosity, "GmGeomVerbosity");

#endif
