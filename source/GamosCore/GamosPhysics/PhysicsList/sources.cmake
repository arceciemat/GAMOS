#------------------------------------------------------------------------------
# 
# Module : GamosCore
# Package: GamosPhysics_PhysicsList
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
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosPhysics_PhysicsList

   HEADERS
       GmPhysicsElectronNuclear.hh
       GmPhysicsPositronStandard.hh
       GmPhysicsCoulombScattering.hh
       GmVPhysicsGamma.hh
       GmPhysicsElectronPenelope.hh
       GmPhysicsElectronStandard.hh
       GmPhysicsGammaLowEner.hh
       GmPhysicsMuonStandard.hh
       GmPhysicsElectronLowEner.hh
       GmPhysicsMultipleScattering.hh
       HadrontherapyPhysicsMessenger.hh
       HadrontherapyStepMaxMessenger.hh
       HadrontherapyStepMax.hh
       GmPhysicsCerenkov.hh
       GmPhysicsRadioactiveDecay.hh
       GmXrayRefraction.hh
       GmPhysicsPositronNuclear.hh
       GmPhysicsGammaNuclear.hh
       GmVPhysicsElectron.hh
       GmPhysicsDecay.hh
       GmPhysicsVerbosity.hh
       GmPhysicsParallel.hh
       GmEMExtendedPhysics.hh
       GmPhysicsXrayRefraction.hh
       GmPhysicsOpticalPhoton.hh
       GmEMStandardPhysicsMessenger.hh
       GmEMPhysics.hh
       GmPhysicsGammaPenelope.hh
       GmPhysicsPositronPenelope.hh
       HadrontherapyPhysics.hh
       GmEMStandardPhysics.hh
       GmPhysicsMessenger.hh
       LocalIonIonInelasticPhysic.hh
       GmPhysicsGammaStandard.hh
       GmExtraPhysicsMessenger.hh

   SOURCES
       plugin.cc
       GmEMExtendedPhysics.cc
       HadrontherapyStepMax.cc
       GmPhysicsPositronNuclear.cc
       GmPhysicsPositronStandard.cc
       GmEMStandardPhysicsMessenger.cc
       GmPhysicsVerbosity.cc
       GmPhysicsElectronPenelope.cc
       HadrontherapyPhysics.cc
       GmPhysicsGammaLowEner.cc
       GmExtraPhysicsMessenger.cc
       GmPhysicsRadioactiveDecay.cc
       GmEMStandardPhysics.cc
       HadrontherapyStepMaxMessenger.cc
       GmPhysicsParallel.cc
       HadrontherapyPhysicsMessenger.cc
       GmPhysicsGammaStandard.cc
       GmPhysicsMultipleScattering.cc
       LocalIonIonInelasticPhysic.cc
       GmPhysicsCoulombScattering.cc
       GmVPhysicsElectron.cc
       GmPhysicsElectronNuclear.cc
       GmPhysicsGammaPenelope.cc
       GmPhysicsPositronPenelope.cc
       GmPhysicsMuonStandard.cc
       GmPhysicsElectronLowEner.cc
       GmPhysicsElectronStandard.cc
       GmPhysicsOpticalPhoton.cc
       GmPhysicsDecay.cc
       GmPhysicsMessenger.cc
       GmPhysicsGammaNuclear.cc
       GmVPhysicsGamma.cc
       GmXrayRefraction.cc
       GmEMPhysics.cc
       GmPhysicsXrayRefraction.cc
       GmPhysicsCerenkov.cc
 
   GRANULAR_DEPENDENCIES

   GLOBAL_DEPENDENCIES
    ${Geant4_LIBRARIES}
    ${Root_LIBRARIES}
  GamosCore_GamosBase_Base
  GamosCore_GamosUtils
  GamosCore_GamosGenerator

   LINK_LIBRARIES
)

# List any source specific properties here
