#include "Reflex/PluginService.h"

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
