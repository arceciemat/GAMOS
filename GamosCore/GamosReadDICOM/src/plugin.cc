#include "Reflex/PluginService.h"

#include "GmReadPhantomG4Geometry.hh"
#include "GmReadPhantomG4BinGeometry.hh"
#include "GmReadPhantomEGSGeometry.hh"
#include "GmSimplePhantomGeometry.hh"
#include "GmReadPhantomEGSwithPSGeometry.hh"
#include "GmReadPhantomG4withPSGeometry.hh"
#include "GmReadPhantomPartialG4Geometry.hh"

PLUGINSVC_FACTORY(GmReadPhantomG4Geometry,G4VUserDetectorConstruction*())
PLUGINSVC_FACTORY(GmReadPhantomG4BinGeometry,G4VUserDetectorConstruction*())
PLUGINSVC_FACTORY(GmReadPhantomEGSGeometry,G4VUserDetectorConstruction*())
PLUGINSVC_FACTORY(GmSimplePhantomGeometry,G4VUserDetectorConstruction*())
PLUGINSVC_FACTORY(GmReadPhantomEGSwithPSGeometry,G4VUserDetectorConstruction*())
PLUGINSVC_FACTORY(GmReadPhantomG4withPSGeometry,G4VUserDetectorConstruction*())
PLUGINSVC_FACTORY(GmReadPhantomPartialG4Geometry,G4VUserDetectorConstruction*())

#include "GmReadDICOMVerbosity.hh"
PLUGINSVC_FACTORY(GmReadDICOMVerbosity,GmVVerbosity*())

#include "GmBuildPhantomStructuresUA.hh"
PLUGINSVC_FACTORY(GmBuildPhantomStructuresUA,GmUserAction*())

#include "GmGenerDistPositionInPhantomVoxels.hh"
PLUGINSVC_FACTORY(GmGenerDistPositionInPhantomVoxels,GmVGenerDistPosition*())

#include "GmPhantomStructures.hh"
PLUGINSVC_FACTORY(GmEnterPhantomStructureFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmExitPhantomStructureFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmStartPhantomStructureFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmEndPhantomStructureFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmTraversePhantomStructureFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmInPhantomStructureFilter,GmVFilter*(G4String))

