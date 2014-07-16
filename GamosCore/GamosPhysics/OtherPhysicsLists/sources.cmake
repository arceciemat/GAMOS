#------------------------------------------------------------------------------
# 
# Module : GamosCore
# Package: GamosPhysics_OtherPhysicsLists
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
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosPhysics_OtherPhysicsLists

   HEADERS
       GmLEPNeutronBuilder_noFission.hh
       GmHadronPhysicsQGSP_BIC_NHP.hh
       GmQGSP_BIC_HPXS.hh
       GmNeutronBuilder_noHPFission.hh
       GmQGSP_BERT_HP_noFission.hh
       GmNeutronHPBuilder_noHPFission.hh
       GmNeutronBuilder_noFission.hh
       GmNeutronHPBuilder_noFission.hh
       GmLEPNeutronBuilder_noHPFission.hh
       GmNeutronHPXSBuilder.hh
       GmDummyPhysicsList.hh
       GmHadronPhysicsQGSP_BERT_HP_noFission.hh
       GmQGSP_BIC_NHP.hh
       GmQGSP_BERT_HP_noHPFission.hh
       GmDummyPhysics.hh
       GmQGSP_BIC_HPThermal.hh
       GmProtonHPBuilder.hh
       GmHadronPhysicsQGSP_BIC_PHP.hh
       GmNeutronNHPBuilder.hh
       GmHadronPhysicsQGSP_BERT_HP_noHPFission.hh
       GmQGSP_BIC_PHP.hh
       GmG4PhysicsLists.hh
       GmDNAPhysics.hh
       GmProtonHPBaseBuilder.hh

   SOURCES
       plugin.cc
       GmProtonHPBuilder.cc
       GmNeutronBuilder_noFission.cc
       GmNeutronBuilder_noHPFission.cc
       GmDummyPhysics.cc
       GmLEPNeutronBuilder_noFission.cc
       GmNeutronNHPBuilder.cc
       GmDNAPhysics.cc
       GmHadronPhysicsQGSP_BERT_HP_noHPFission.cc
       GmProtonHPBaseBuilder.cc
       GmNeutronHPBuilder_noHPFission.cc
       GmNeutronHPBuilder_noFission.cc
       GmQGSP_BERT_HP_noFission.cc
       GmHadronPhysicsQGSP_BIC_PHP.cc
       GmDummyPhysicsList.cc
       GmNeutronHPXSBuilder.cc
       GmLEPNeutronBuilder_noHPFission.cc
       GmQGSP_BERT_HP_noHPFission.cc
       GmHadronPhysicsQGSP_BIC_NHP.cc
       GmHadronPhysicsQGSP_BERT_HP_noFission.cc
 
   GRANULAR_DEPENDENCIES

   GLOBAL_DEPENDENCIES
    ${Geant4_LIBRARIES}
    ${Root_LIBRARIES}

   LINK_LIBRARIES
)

# List any source specific properties here
