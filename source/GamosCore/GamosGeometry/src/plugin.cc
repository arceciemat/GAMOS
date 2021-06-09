#include "Reflex/PluginService.h"
//#include "GamosCore/GamosGeometry/include/GDMLDetectorConstruction.hh"

#include "GmGeometryFromText.hh"
#include "GmGeomTextDumperUA.hh"
#include "GmGeometryUseMaterials.hh"

//PLUGINSVC_FACTORY (GDMLDetectorConstruction);
PLUGINSVC_FACTORY (GmGeometryFromText,G4VUserDetectorConstruction*())
//PLUGINSVC_FACTORY (GmGeometryFromTextOld);
PLUGINSVC_FACTORY(GmGeometryUseMaterials,G4VUserDetectorConstruction*())

PLUGINSVC_FACTORY(GmGeomTextDumperUA,GmUserAction*())

#include "GmGeomVerbosity.hh"
PLUGINSVC_FACTORY(GmGeomVerbosity,GmVVerbosity*())
 
