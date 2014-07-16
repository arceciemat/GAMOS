#------------------------------------------------------------------------------
# 
# Module : GamosCore
# Package: GamosPhysics_VarianceReduction
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
GEANT4_DEFINE_MODULE(NAME GamosCore_GamosPhysics_VarianceReduction

   HEADERS
       GmEWPSeeToTwoGammaModel.hh
       GmBSTrajectory.hh
       GmPSeIonisation.hh
       GmEWPSLivermoreComptonModel.hh
       GmEWPSKleinNishinaComptonModel.hh
       GmPhysicsGammaEWPSLowEner.hh
       GmDeexSplitLivermorePhotoElectricModel.hh
       GmPhysicsPositronUBSStandard.hh
       GmDeexSpliteIonisation.hh
       GmUBSLivermoreBremsstrahlungModel.hh
       GmPhysicsElectronEWPSLowEner.hh
       GmPSZPlaneDirCheckerAcceptAll.hh
       GmPSGammaConversion.hh
       GmSplitWrapperProcess.hh
       GmPSEMPhysics.hh
       GmPhysicsElectronLowEnerZBSDeexSplit.hh
       GmPhysicsElectronLowEnerDeexSplit.hh
       GmPSeplusAnnihilation.hh
       GmVDeexSplitEnergyLossProcess.hh
       GmDeexSplitPenelopeComptonModel.hh
       GmEWPSLivermorePhotoElectricModel.hh
       GmPhysicsElectronPenelopeDeexSplit.hh
       GmPhysicsGammaEWPSStandard.hh
       GmWrapperProcess.hh
       GmEWPSPEEffectModel.hh
       GmEWPSBetheHeitlerModel.hh
       GmPhysicsGammaPenelopeDeexSplit.hh
       GmPSZPlaneDirChecker.hh
       GmPhysicsPositronZBSStandard.hh
       GmPhysicsPositronEWPSStandard.hh
       GmPhysicsGammaLowEnerDeexSplit.hh
       GmPhysicsElectronZBSLowEner.hh
       GmVarianceReductionMessenger.hh
       GmBSScanWeightsUA.hh
       GmImportanceSamplingProcess.hh
       GmDeexSplitPenelopePhotoElectricModel.hh
       GmBSControlHistosUA.hh
       GmEWPSLivermoreBremsstrahlungModel.hh
       GmPhysicsElectronUBSLowEner.hh
       GmPhysicsElectronLowEnerUBSDeexSplit.hh
       GmVPSModelHelper.hh
       GmInheritTrackInfoUA.hh
       GmVPSGammaProcess.hh
       GmPSPhotoElectricEffect.hh
       GmEWPSBremsstrahlungModel.hh
       GmBSLivermoreBremsstrahlungModel.hh
       GmPSeBremsstrahlung.hh
       GmDeexSplitLivermoreComptonModel.hh
       GmPSComptonScattering.hh
       GmVPSEnergyLossProcess.hh

   SOURCES
       plugin.cc
       GmPSeBremsstrahlung.cc
       GmPSZPlaneDirCheckerAcceptAll.cc
       GmPhysicsElectronUBSLowEner.cc
       GmPhysicsElectronZBSLowEner.cc
       GmPSEMPhysics.cc
       GmWrapperProcess.cc
       GmPhysicsGammaLowEnerDeexSplit.cc
       GmPhysicsElectronPenelopeDeexSplit.cc
       GmVDeexSplitEnergyLossProcess.cc
       GmEWPSPEEffectModel.cc
       GmEWPSLivermoreBremsstrahlungModel.cc
       GmPhysicsPositronEWPSStandard.cc
       GmBSScanWeightsUA.cc
       GmPhysicsGammaEWPSLowEner.cc
       GmVPSEnergyLossProcess.cc
       GmPSeIonisation.cc
       GmPhysicsElectronLowEnerDeexSplit.cc
       GmDeexSpliteIonisation.cc
       GmPhysicsElectronLowEnerUBSDeexSplit.cc
       GmVPSGammaProcess.cc
       GmSplitWrapperProcess.cc
       GmVarianceReductionMessenger.cc
       GmDeexSplitLivermorePhotoElectricModel.cc
       GmPSPhotoElectricEffect.cc
       GmEWPSBetheHeitlerModel.cc
       GmEWPSLivermorePhotoElectricModel.cc
       GmPhysicsElectronEWPSLowEner.cc
       GmPhysicsElectronLowEnerZBSDeexSplit.cc
       GmPhysicsPositronUBSStandard.cc
       GmPhysicsPositronZBSStandard.cc
       GmPSComptonScattering.cc
       GmBSControlHistosUA.cc
       GmPhysicsGammaPenelopeDeexSplit.cc
       GmPSZPlaneDirChecker.cc
       GmEWPSeeToTwoGammaModel.cc
       GmVPSModelHelper.cc
       GmEWPSLivermoreComptonModel.cc
       GmBSTrajectory.cc
       GmDeexSplitPenelopeComptonModel.cc
       GmEWPSBremsstrahlungModel.cc
       GmEWPSKleinNishinaComptonModel.cc
       GmDeexSplitPenelopePhotoElectricModel.cc
       GmPSGammaConversion.cc
       GmDeexSplitLivermoreComptonModel.cc
       GmImportanceSamplingProcess.cc
       GmPhysicsGammaEWPSStandard.cc
       GmPSeplusAnnihilation.cc
       GmBSLivermoreBremsstrahlungModel.cc
       GmInheritTrackInfoUA.cc
 
   GRANULAR_DEPENDENCIES

   GLOBAL_DEPENDENCIES
    ${Geant4_LIBRARIES}
    ${Root_LIBRARIES}
  GamosCore_GamosAnalysis
  GamosCore_GamosPhysics_PhysicsList
  GamosCore_GamosGeometry
  GamosCore_GamosUtils
  GamosCore_GamosUserActionMgr
  GamosCore_GamosBase_Base

   LINK_LIBRARIES
)

# List any source specific properties here
