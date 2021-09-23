#include "GmClassifierBy1Ancestor.hh"
#include "GmClassifierByAncestors.hh"
#include "GmClassifierByLogicalVolume.hh"
#include "GmClassifierByKineticEnergy.hh"
#include "GmClassifierByPhysicalVolume.hh"
#include "GmClassifierByPhysicalVolumeReplicated.hh"
#include "GmClassifierByRegion.hh"
#include "GmClassifierByParticle.hh"
#include "GmClassifierByPrimaryParticle.hh"
#include "GmClassifierByProcess.hh"
#include "GmClassifierByCreatorProcess.hh"
#include "GmClassifierByParticleProcess.hh"
#include "GmClassifierByMaterial.hh"
#include "GmCompoundClassifier.hh"
#include "GmClassifierBySecondariesList.hh"
#include "GmClassifierBySecondariesListExclude.hh"
#include "GmClassifierByHadronicReaction.hh"
#include "GmClassifierByParallelLogicalVolume.hh"

#ifdef ROOT5

#include "Reflex/PluginService.h"
PLUGINSVC_FACTORY(GmClassifierByAncestors,GmVClassifier*(G4String))
PLUGINSVC_FACTORY(GmClassifierBy1Ancestor,GmVClassifier*(G4String))
PLUGINSVC_FACTORY(GmClassifierByLogicalVolume,GmVClassifier*(G4String))
PLUGINSVC_FACTORY(GmClassifierByKineticEnergy,GmVClassifier*(G4String))
PLUGINSVC_FACTORY(GmClassifierByPhysicalVolume,GmVClassifier*(G4String))
PLUGINSVC_FACTORY(GmClassifierByPhysicalVolumeReplicated,GmVClassifier*(G4String))
PLUGINSVC_FACTORY(GmClassifierByRegion,GmVClassifier*(G4String))
PLUGINSVC_FACTORY(GmClassifierByParticle,GmVClassifier*(G4String))
PLUGINSVC_FACTORY(GmClassifierByPrimaryParticle,GmVClassifier*(G4String))
PLUGINSVC_FACTORY(GmClassifierByProcess,GmVClassifier*(G4String))
PLUGINSVC_FACTORY(GmClassifierByCreatorProcess,GmVClassifier*(G4String))
PLUGINSVC_FACTORY(GmClassifierByParticleProcess,GmVClassifier*(G4String))
PLUGINSVC_FACTORY(GmClassifierByMaterial,GmVClassifier*(G4String))
PLUGINSVC_FACTORY(GmCompoundClassifier,GmVClassifier*(G4String))
PLUGINSVC_FACTORY(GmClassifierBySecondariesList,GmVClassifier*(G4String))
PLUGINSVC_FACTORY(GmClassifierBySecondariesListExclude,GmVClassifier*(G4String))
PLUGINSVC_FACTORY(GmClassifierByHadronicReaction,GmVClassifier*(G4String))
PLUGINSVC_FACTORY(GmClassifierByParallelLogicalVolume,GmVClassifier*(G4String))

#include "GmClassifierVerbosity.hh"
PLUGINSVC_FACTORY(GmClassifierVerbosity, GmVVerbosity*())

#else 
#include "GamosCore/GamosBase/Base/include/GmClassifierFactory.hh"

DEFINE_SEAL_MODULE ();

DEFINE_GAMOS_CLASSIFIER(GmClassifierByAncestors);
DEFINE_GAMOS_CLASSIFIER(GmClassifierBy1Ancestor);
DEFINE_GAMOS_CLASSIFIER(GmClassifierByLogicalVolume);
DEFINE_GAMOS_CLASSIFIER(GmClassifierByKineticEnergy);
DEFINE_GAMOS_CLASSIFIER(GmClassifierByPhysicalVolume);
DEFINE_GAMOS_CLASSIFIER(GmClassifierByPhysicalVolumeReplicated);
DEFINE_GAMOS_CLASSIFIER(GmClassifierByRegion);
DEFINE_GAMOS_CLASSIFIER(GmClassifierByParticle);
DEFINE_GAMOS_CLASSIFIER(GmClassifierByPrimaryParticle);
DEFINE_GAMOS_CLASSIFIER(GmClassifierByProcess);
DEFINE_GAMOS_CLASSIFIER(GmClassifierByCreatorProcess);
DEFINE_GAMOS_CLASSIFIER(GmClassifierByParticleProcess);
DEFINE_GAMOS_CLASSIFIER(GmClassifierByMaterial);
DEFINE_GAMOS_CLASSIFIER(GmCompoundClassifier);
DEFINE_GAMOS_CLASSIFIER(GmClassifierBySecondariesList);
DEFINE_GAMOS_CLASSIFIER(GmClassifierBySecondariesListExclude);
DEFINE_GAMOS_CLASSIFIER(GmClassifierByHadronicReaction);
DEFINE_GAMOS_CLASSIFIER(GmClassifierByParallelLogicalVolume);

#include "GamosCore/GamosBase/Base/include/GmVerbosityFactory.hh"
#include "GmClassifierVerbosity.hh"
DEFINE_SEAL_PLUGIN(GmVerbosityFactory, GmClassifierVerbosity, "GmClassifierVerbosity");

#endif
