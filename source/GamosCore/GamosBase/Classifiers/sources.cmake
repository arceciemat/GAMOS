#------------------------------------------------------------------------------
# 
# Module : GamosCore
# Package: GamosBase_Classifiers
#
#------------------------------------------------------------------------------
# List external includes needed.

# List external includes needed.
include(GamosSetup)
include(UseRoot)

#
# Define the GAMOS Module.
#
include(Geant4MacroDefineModule)
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosBase_Classifiers

   HEADERS
       GmClassifierByPhysicalVolume.hh
       GmClassifierByProcess.hh
       GmClassifierByMaterial.hh
       GmClassifierByParticleProcess.hh
       GmClassifierByRegion.hh
       GmClassifierBy1Ancestor.hh
       GmClassifierBySecondariesListExclude.hh
       GmClassifierByLogicalVolume.hh
       GmClassifierByPhysicalVolumeReplicated.hh
       GmClassifierByCreatorProcess.hh
       GmClassifierByPrimaryParticle.hh
       GmClassifierByParticle.hh
       GmClassifierByAncestors.hh
       GmClassifierBySecondariesList.hh
       GmClassifierByHadronicReaction.hh
       GmCompoundClassifier.hh
       GmClassifierByKineticEnergy.hh

   SOURCES
       plugin.cc
       GmClassifierByProcess.cc
       GmClassifierByPhysicalVolume.cc
       GmClassifierBySecondariesList.cc
       GmClassifierByKineticEnergy.cc
       GmClassifierByLogicalVolume.cc
       GmClassifierByParticleProcess.cc
       GmClassifierByParticle.cc
       GmClassifierByPhysicalVolumeReplicated.cc
       GmClassifierByCreatorProcess.cc
       GmClassifierByPrimaryParticle.cc
       GmClassifierByAncestors.cc
       GmClassifierByRegion.cc
       GmClassifierByMaterial.cc
       GmCompoundClassifier.cc
       GmClassifierByHadronicReaction.cc
       GmClassifierBySecondariesListExclude.cc
       GmClassifierBy1Ancestor.cc
 
   GRANULAR_DEPENDENCIES

   GLOBAL_DEPENDENCIES
    ${Geant4_LIBRARIES}
    ${Root_LIBRARIES}
  GamosCore_GamosBase_Base
  GamosCore_GamosUtils
  GamosCore_GamosGeometry

   LINK_LIBRARIES
)

# List any source specific properties here
