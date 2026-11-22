#include "GmReadPhantomG4Geometry.hh"
#include "GmReadPhantomG4BinGeometry.hh"
#include "GmReadPhantomEGSGeometry.hh"
#include "GmSimplePhantomGeometry.hh"
#include "GmReadPhantomEGSwithStGeometry.hh"
#include "GmReadPhantomG4withStGeometry.hh"
#include "GmReadPhantomPartialG4Geometry.hh"
#include "GmCrossPhantomGeometry.hh"

#include "GmReadDICOMVerbosity.hh"
#include "GmBuildPhantomStructuresUA.hh"
#include "GmDumpMateG4dcmUA.hh"
#include "GmGenerDistPositionInPhantomVoxels.hh"
#include "GmGenerDistPositionInStPhantomVoxels.hh"
#include "GmGenerDistPositionPETImage.hh"
#include "GmGenerDistPositionNMImage.hh"
#include "GmPhantomStructures.hh"
#include "GmClassifierByPhantomStructure.hh"
#include "GmClassifierCrossPhantom.hh"

#ifdef ROOT5
#include "Reflex/PluginService.h"

PLUGINSVC_FACTORY(GmReadPhantomG4Geometry,G4VUserDetectorConstruction*())
PLUGINSVC_FACTORY(GmReadPhantomG4BinGeometry,G4VUserDetectorConstruction*())
PLUGINSVC_FACTORY(GmReadPhantomEGSGeometry,G4VUserDetectorConstruction*())
PLUGINSVC_FACTORY(GmSimplePhantomGeometry,G4VUserDetectorConstruction*())
PLUGINSVC_FACTORY(GmReadPhantomEGSwithStGeometry,G4VUserDetectorConstruction*())
PLUGINSVC_FACTORY(GmReadPhantomG4withStGeometry,G4VUserDetectorConstruction*())
PLUGINSVC_FACTORY(GmReadPhantomPartialG4Geometry,G4VUserDetectorConstruction*())
PLUGINSVC_FACTORY(GmCrossPhantomGeometry,G4VUserDetectorConstruction*())

PLUGINSVC_FACTORY(GmReadDICOMVerbosity,GmVVerbosity*())

PLUGINSVC_FACTORY(GmBuildPhantomStructuresUA,GmUserAction*())
PLUGINSVC_FACTORY(GmDumpMateG4dcmUA,GmUserAction*())

PLUGINSVC_FACTORY(GmGenerDistPositionInPhantomVoxels,GmVGenerDistPosition*())
PLUGINSVC_FACTORY(GmGenerDistPositionInStPhantomVoxels,GmVGenerDistPosition*())
PLUGINSVC_FACTORY(GmGenerDistPositionPETImage,GmVGenerDistPosition*())
PLUGINSVC_FACTORY(GmGenerDistPositionNMImage,GmVGenerDistPosition*())

PLUGINSVC_FACTORY(GmEnterPhantomStructureFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmExitPhantomStructureFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmStartPhantomStructureFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmEndPhantomStructureFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmTraversePhantomStructureFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmInPhantomStructureFilter,GmVFilter*(G4String))
PLUGINSVC_FACTORY(GmClassifierByPhantomStructure,GmVClassifier*(G4String))

PLUGINSVC_FACTORY(GmClassifierCrossPhantom,GmVClassifier*(G4String))

#else

#include "GamosCore/GamosGeometry/include/GmGeometryFactory.hh"
#include "GamosCore/GamosGenerator/include/GmGeneratorDistributionFactories.hh"
#include "GamosCore/GamosUserActionMgr/include/GmUserActionFactory.hh"
#include "GamosCore/GamosBase/Base/include/GmFilterFactory.hh"
#include "GamosCore/GamosBase/Base/include/GmClassifierFactory.hh"
#include "SEAL_Foundation/PluginManager/PluginManager/ModuleDef.h"

DEFINE_SEAL_MODULE ();

DEFINE_GAMOS_GEOMETRY(GmReadPhantomG4Geometry);
DEFINE_GAMOS_GEOMETRY(GmReadPhantomG4BinGeometry);
DEFINE_GAMOS_GEOMETRY(GmReadPhantomEGSGeometry);
DEFINE_GAMOS_GEOMETRY(GmSimplePhantomGeometry);
DEFINE_GAMOS_GEOMETRY(GmReadPhantomEGSwithStGeometry);
DEFINE_GAMOS_GEOMETRY(GmReadPhantomG4withStGeometry);
DEFINE_GAMOS_GEOMETRY(GmReadPhantomPartialG4Geometry);
DEFINE_GAMOS_GEOMETRY(GmCrossPhantomGeometry);

DEFINE_GAMOS_USER_ACTION(GmBuildPhantomStructuresUA);

DEFINE_GAMOS_GENER_DIST_POSITION(GmGenerDistPositionInPhantomVoxels);
DEFINE_GAMOS_GENER_DIST_POSITION(GmGenerDistPositionInStPhantomVoxels);
DEFINE_GAMOS_GENER_DIST_POSITION(GmGenerDistPositionPETImage);
DEFINE_GAMOS_GENER_DIST_POSITION(GmGenerDistPositionNMImage);

DEFINE_GAMOS_FILTER(GmEnterPhantomStructureFilter);
DEFINE_GAMOS_FILTER(GmExitPhantomStructureFilter);
DEFINE_GAMOS_FILTER(GmStartPhantomStructureFilter);
DEFINE_GAMOS_FILTER(GmEndPhantomStructureFilter);
DEFINE_GAMOS_FILTER(GmTraversePhantomStructureFilter);
DEFINE_GAMOS_FILTER(GmInPhantomStructureFilter);
DEFINE_GAMOS_CLASSIFIER(GmClassifierByPhantomStructure);

DEFINE_GAMOS_CLASSIFIER(GmClassifierCrossPhantom);

DEFINE_GAMOS_USER_ACTION(GmDumpMateG4dcmUA);

#include "GamosCore/GamosBase/Base/include/GmVerbosityFactory.hh"
#include "GmReadDICOMVerbosity.hh"
DEFINE_GAMOS_VERBOSITY(GmReadDICOMVerbosity);


#endif

