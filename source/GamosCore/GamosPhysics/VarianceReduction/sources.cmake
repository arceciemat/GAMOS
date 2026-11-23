# - GamosCore_GamosPhysics_VarianceReduction module build definition

gamos_add_module(GamosCore_GamosPhysics_VarianceReduction
  PUBLIC_HEADERS
    GmDeexSplitLivermoreComptonModel.hh
    GmDeexSplitLivermorePhotoElectricModel.hh
    GmDeexSplitPenelopeComptonModel.hh
    GmDeexSplitPenelopePhotoElectricModel.hh
    GmDeexSpliteIonisation.hh
    GmImportanceSamplingProcess.hh
    GmInheritTrackInfoUA.hh
    GmPhysicsElectronLowEnerDeexSplit.hh
    GmPhysicsElectronLowEnerUBSDeexSplit.hh
    GmPhysicsElectronPenelopeDeexSplit.hh
    GmPhysicsGammaLowEnerDeexSplit.hh
    GmPhysicsGammaPenelopeDeexSplit.hh
    GmSplitWrapperProcess.hh
    GmVDeexSplitEnergyLossProcess.hh
    GmVarianceReductionMessenger.hh
    GmWrapperProcess.hh
  SOURCES
    GmImportanceSamplingProcess.cc
    GmInheritTrackInfoUA.cc
    GmSplitWrapperProcess.cc
    GmVarianceReductionMessenger.cc
    GmWrapperProcess.cc
)

gamos_module_link_libraries(GamosCore_GamosPhysics_VarianceReduction
  PUBLIC
    GamosCore_GamosBase_Base
    GamosCore_GamosPhysics_PhysicsList
    GamosCore_GamosUserActionMgr
    GamosCore_GamosUtils
)
