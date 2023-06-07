# - GamosCore_GamosPhysics_VarianceReduction module build definition

include(UseGamosAtGeant4)
include(UseROOT)

geant4_add_module(GamosCore_GamosPhysics_VarianceReduction
  PUBLIC_HEADERS
    GmWrapperProcess.hh
    GmPhysicsElectronLowEnerUBSDeexSplit.hh
    GmPhysicsElectronLowEnerDeexSplit.hh
    GmDeexSplitLivermoreComptonModel.hh
    GmPhysicsGammaPenelopeDeexSplit.hh
    GmDeexSpliteIonisation.hh
    GmSplitWrapperProcess.hh
    GmDeexSplitPenelopeComptonModel.hh
    GmVarianceReductionMessenger.hh
    GmImportanceSamplingProcess.hh
    GmDeexSplitPenelopePhotoElectricModel.hh
    GmPhysicsElectronPenelopeDeexSplit.hh
    GmVDeexSplitEnergyLossProcess.hh
    GmDeexSplitLivermorePhotoElectricModel.hh
    GmInheritTrackInfoUA.hh
    GmPhysicsGammaLowEnerDeexSplit.hh
  SOURCES
    GmSplitWrapperProcess.cc
    GmInheritTrackInfoUA.cc
    GmImportanceSamplingProcess.cc
    GmWrapperProcess.cc
    GmVarianceReductionMessenger.cc
)
# - Add path to generated header
geant4_module_include_directories(GamosCore_GamosPhysics_VarianceReduction
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/SealBase/include>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/SealPlatform/include>
 PUBLIC  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/source/GAMOS63/SEAL_Foundation/PluginManager/include>
 PUBLIC  $<BUILD_INTERFACE:${ROOT_BASE_DIR}/include>)

geant4_module_link_libraries(GamosCore_GamosPhysics_VarianceReduction
  PUBLIC
    ${Geant4_libs}
    ${ROOT_LIBRARIES}
    ${SEAL_LIBRARIES} 
    GamosCore_GamosUtils
    GamosCore_GamosGeometry
    GamosCore_GamosPhysics_PhysicsList
    GamosCore_GamosUserActionMgr
    GamosCore_GamosUtils
)
